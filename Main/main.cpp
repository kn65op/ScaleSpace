#include <ScaleSpaceFactory.h>
#include <Camera.h>
#include <CameraException.h>
#include "ProgramController.h"
#include "HelperFunctions.h"

#include <opencv\cv.h>
#include <opencv\highgui.h>

#include <iostream>
#include <fstream>

int main(int argc, char * argv[])
{
 // Stoper prepare, 
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

  cv::Mat input;
  ScaleSpaceImage output;
  try
  {
    ScaleSpace *ss = ScaleSpaceFactory::getScaleSpace(controller.getProcessor(), controller.getMode());
    ss->setScaleStep(controller.getScaleStep(), controller.getNrScales());
    ss->prepare(controller.getSourceImageType(), ScaleSpaceOutputType::IMAGE_FOR_SCALE);
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
    ss->processImage(output);

    output.show("result");
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
  return 0;
}
