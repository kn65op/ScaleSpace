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
    prepare.start();
    ScaleSpace *ss = ScaleSpaceFactory::getScaleSpace(controller.getProcessor(), controller.getMode());
    ss->setScaleStep(controller.getScaleStep(), controller.getNrScales());
    ss->prepare(controller.getSourceImageType(), ScaleSpaceOutputType::IMAGE_FOR_SCALE);
    prepare.stop();
    if (controller.useCamera())
    {
      camera = JAI::Camera::getCameraList().front();
      input = camera->getNextFrame();
    }
    else
    {
      input = cv::imread(controller.getInputFile().c_str(), CV_LOAD_IMAGE_GRAYSCALE);
    }
    if (!input.size().width)
    {
      std::cout << "Input file is not valid image - not processing.\n";
      return 0;
    }
      
    output.setInput(input);
    process.start();
    ss->processImage(output);
    process.stop();


    output.show(controller.getOutputPrefix(), getStringFromScaleSpaceProcessor(controller.getProcessor()));
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
  std::cout << "Program took: " << all.stop() << " " << all.getUnitName() << "\n";
  std::cout << "Prepare took: " << prepare.getTime() << " " << all.getUnitName() << "\n";
  std::cout << "Process took: " << process.getTime() << " " << all.getUnitName() << "\n";
  try
  {
    std::cout << "Gaussians took: " << Stoper::getTime("gaussian") << " " << Stoper::getUnitName() << "\n";
    for (unsigned int i=0; i < controller.getNrScales(); ++i)
    {
      std::cout << "Gaussian " << i << " took: " << Stoper::getTime("gaussian" + std::to_string(i)) << " " << Stoper::getUnitName() << "\n";
    }
  }
  catch(StoperException & ex)
  {
    std::cout << (std::string)ex << "\n";
  }

  return 0;
}
