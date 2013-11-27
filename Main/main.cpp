#include <ScaleSpaceFactory.h>
#include <Camera.h>
#include <CameraException.h>
#include <Stoper.h>
#include <DeviceSelector.h>

#include "ProgramController.h"
#include "HelperFunctions.h"

#include <opencv\cv.h>
#include <opencv\highgui.h>

#include <iostream>
#include <fstream>
#include <thread>

using namespace TTime;

int main(int argc, char * argv[])
{
  bool exception = false;
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
  ScaleSpaceParameters ssp;
  ssp.calc_first_image = controller.calcFirstImage();
  ssp.debug = controller.isDebug();
  ssp.quiet = controller.isQuiet();
  ssp.device = controller.getOpenCL();
  try
  {
    prepare.start(false);
    ScaleSpace *ss = ScaleSpaceFactory::getScaleSpace(controller.getProcessor(), controller.getMode());
    ss->setScaleStep(controller.getScaleStep(), controller.getNrScales());
    ss->setParameters(ssp);

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
      processScaleSpace(ss, controller, input, output, process, controller.isShow(), 0, controller.calcFirstImage(), controller.isDebug());
    }
    else if (controller.useCamera()) //process all camera
    {
      unsigned int i = 0;
      camera = JAI::Camera::getCameraList().front();
      camera->open();
      camera->start();
      std::this_thread::sleep_for(std::chrono::seconds(1)); //Wait for camera to start working
    
      while (1) //TODO: Stop condition
      {
        input = camera->getNextFrame();
        processScaleSpace(ss, controller, input, output, process, controller.isShow(), i++, controller.calcFirstImage(), controller.isDebug());
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
        input = readFileFromDir(prefix, i++);
        if (!input.size().width)
        {
          is_image = false;
        }
        else
        {
          processScaleSpace(ss, controller, input, output, process, controller.isShow(), in++, controller.calcFirstImage(), controller.isDebug());
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
    exception = true;
    std::cout << "OpenCLDeviceException: " << ex.getFullMessage() << "\n";
    std::cout << "Source:\n" << ex.getSource() << "\n";
  }
  catch(OpenCLException &ex)
  {
    exception = true;
    std::cout << "OpenCLException: " << ex.getFullMessage() << "\n";
  }
  catch (NoPlatformScaleSpaceException &ex)
  {
    exception = true;
    std::cout << "Platform for this processor is no available:\n\t" << (std::string)ex << "\n";
  }
  catch (ScaleSpaceException &ex)
  {
    exception = true;
    std::cout << "ScaleSpaceExcetpion: " << (std::string)ex << "\n";
  }
  catch (ScaleSpaceImageException &ex)
  {
    exception = true;
    std::cout << "ScaleSpaceImageException: " << (std::string)ex << "\n";
  }
  catch (JAI::CameraException &ex)
  {
    exception = true;
    std::cout << (std::string)ex << "\n";
    std::cout << "JAI camera is not connected\n";
  }
  catch (DeviceSelector::NotFoundDeviceException &ex)
  {
    exception = true;
    std::cout << "Specified device: " << controller.getOpenCL() << " not found\n";
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
    exception = true;
    std::cout << (std::string)ex << "\n";
  }
  out << "\n";

  if (exception)
  {
    std::ofstream out_err("err" + controller.getOutputPrefix() + ".txt");
    out_err << "Exception occured\n";
  }

  return 0;
}
