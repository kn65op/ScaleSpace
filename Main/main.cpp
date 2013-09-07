#include <ScaleSpaceFactory.h>
#include <Camera.h>
#include <CameraException.h>
#include <Stoper.h>

#include "ProgramController.h"
#include "HelperFunctions.h"

#include <opencv\cv.h>
#include <opencv\highgui.h>

#include <iostream>
#include <fstream>

using namespace TTime;

int main(int argc, char * argv[])
{
  Stoper prepare, process, all;
  all.start();
  JAI::Camera *camera = nullptr;
  ProgramController controller(argc, argv);
  if (!controller.areOptionsValid())
  {
    controller.printHelp();
    return 0;
  }
  if (controller.isDeviceInfo())
  {
    printCLDeviceInfo();
    return 0;
  }
  controller.printProgramInfo();

  cv::Mat input;
  ScaleSpaceImage output;
  try
  {
    prepare.start(false);
    ScaleSpace *ss = ScaleSpaceFactory::getScaleSpace(controller.getProcessor(), controller.getMode());
    ss->setScaleStep(controller.getScaleStep(), controller.getNrScales());
    ss->prepare(controller.getSourceImageType(), ScaleSpaceOutputType::IMAGE_FOR_SCALE);
    prepare.stop();
    if (controller.oneInputFile())
    {
      input = cv::imread(controller.getInputFile().c_str(), CV_LOAD_IMAGE_GRAYSCALE);
      if (!input.size().width)
      {
        std::cout << "Input file is not valid image - not processing.\n";
        return 0;
      }
      processScaleSpace(ss, controller, input, output, process, controller.isShow(), 0, controller.calcFirstImage());
    }
    else if (controller.useCamera()) //process all camera
    {
      unsigned int i = 0;
      while (1) //TODO: Stop condition
      {
        camera = JAI::Camera::getCameraList().front();
        input = camera->getNextFrame();
        processScaleSpace(ss, controller, input, output, process, controller.isShow(), i++, controller.calcFirstImage());
      }
    }
    else
    {
      std::string prefix = controller.getInputPrefix();

      unsigned int i = 0;
      unsigned int in = 0;
      //while ((input = cv::imread(getFileWithPrefix(prefix, i++))),  input.size().width)
      bool is_image = true;
      while (is_image)
      {
        input = cv::imread(getFileWithPrefix(prefix, i++).c_str(), CV_LOAD_IMAGE_GRAYSCALE);
        if (!input.size().width)
        {
          is_image = false;
        }
        else
        {
          processScaleSpace(ss, controller, input, output, process, controller.isShow(), in++, controller.calcFirstImage());
        }
      }
      if (!controller.isQuiet())
      {
        std::cout << "End of input files. Last file was: " << getFileWithPrefix(prefix, i - 2) << "\n";
      }
    }

  }
  catch (OpenCLDeviceException &ex)
  {
    std::cout << "OpenCLDeviceException: " << ex.getFullMessage() << "\n";
    std::cout << "Source:\n" << ex.getSource() << "\n";
  }
  catch(OpenCLException &ex)
  {
    std::cout << "OpenCLException: " << ex.getFullMessage() << "\n";
  }
  catch (NoPlatformScaleSpaceException &ex)
  {
    std::cout << "Platform for this processor is no available:\n\t" << (std::string)ex << "\n";
  }
  catch (ScaleSpaceException &ex)
  {
    std::cout << "ScaleSpaceExcetpion: " << (std::string)ex << "\n";
  }
  catch (ScaleSpaceImageException &ex)
  {
    std::cout << "ScaleSpaceImageException: " << (std::string)ex << "\n";
  }
  catch (JAI::CameraException &ex)
  {
    std::cout << (std::string)ex << "\n";
    std::cout << "JAI camera is not connected\n";
  }
  all.stop();
  std::fstream out(controller.getOutputPrefix() + ".txt", std::ios_base::app | std::ios_base::out);

  out << getStringFromScaleSpaceProcessor(controller.getProcessor()) << ";";
  out << all.getTime() << ";" << prepare.getTime() << ";" << process.getTime();
  if (!controller.isQuiet())
  {
    std::cout << "Program took: " << all.getTime() << " " << all.getUnitName() << "\n";
    std::cout << "Prepare took: " << prepare.getTime() << " " << all.getUnitName() << "\n";
    std::cout << "Process took: " << process.getTime() << " " << all.getUnitName() << "\n";
  }
  try
  {
    out << ";" << Stoper::getTime("gaussian");
    if (!controller.isQuiet())
    {
      std::cout << "Gaussians took: " << Stoper::getTime("gaussian") << " " << Stoper::getUnitName() << "\n";
    }
    for (unsigned int i=0; i < controller.getNrScales(); ++i)
    {
      out << ";" << Stoper::getTime("gaussian" + std::to_string(i));
      if (!controller.isQuiet())
      {
        std::cout << "Gaussian " << i << " took: " << Stoper::getTime("gaussian" + std::to_string(i)) << " " << Stoper::getUnitName() << "\n";
      }
    }
  }
  catch(StoperException & ex)
  {
    std::cout << (std::string)ex << "\n";
  }
  out << "\n";

  return 0;
}
