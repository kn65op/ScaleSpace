#include <ScaleSpace.h>
#include <Camera.h>
#include "ProgramController.h"

#include <opencv\cv.h>
#include <opencv\highgui.h>

#include <iostream>

//#define LIST_IMAGES

#ifdef LIST_IMAGES
#include <OpenCLHelpers.h>
#endif

#ifdef OLD

#include <opencv\cv.h>

#include <OpenCLGaussian.h>
#include <OpenCLIntToFloat.h>
#include <OpenCLFloatToInt.h>
#endif

#include <fstream>

int main(int argc, char * argv[])
{
  JAI::Camera *camera = nullptr;
  ProgramController controller(argc, argv);
  if (!controller.areOptionsValid())
  {
    controller.printHelp();
    return 0;
  }

  cv::Mat input;
  ScaleSpaceImage output;
  try
  {
    ScaleSpace ss/*;//*/(controller.getMode());
    ss.setScaleStep(controller.getScaleStep(), controller.getNrScales());
    ss.prepare();
    if (controller.useCamera())
    {
      camera = JAI::Camera::getCameraList().front();
    }
    else
    {
      input = cv::imread(controller.getInputFile().c_str(), CV_LOAD_IMAGE_GRAYSCALE);
      if (!input.size().width)
      {
        std::cout << "File is not valid image\n";
        return 0;
      }
      ss.processImage(input, output);
    }

//    output.show();
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
  return 0;
}

#if 0 //old code can be useful for debugging


#ifdef LIST_IMAGES
  list_devices();
  list_supported_image_formats();
  return 0;
#endif

#ifndef OLD
  cv::Mat input;
  input = cv::imread("in.bmp", CV_LOAD_IMAGE_GRAYSCALE);

  ScaleSpaceImage output;
  try
  {
    ScaleSpace ss/*;//*/(ScaleSpaceMode::Laplacian);
    ss.setScaleStep(8, 20);
    ss.prepare();
    /*ss.processImage(input, output);

    output.show();
    input = cv::imread("in2.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    ss.processImage(input, output);

    output.show();
    
    input = cv::imread("in3.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    ss.processImage(input, output);

    output.show();
    */
    input = cv::imread("in4.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    ss.processImage(input, output);

//    output.show();
  }
  catch(OpenCLException &ex)
  {
    std::cout << ex.getFullMessage() << "\n";
  }
  catch (ScaleSpaceException &ex)
  {
    std::cout << (std::string)ex << "\n";
  }
  catch (ScaleSpaceImageException &ex)
  {
    std::cout << (std::string)ex << "\n" << "Image\n";
  }
  return 0;
#endif

#ifdef OLD
  int a = 0;
  try
  {
    OpenCLAlgorithmsStream gaussian_stream;
    OpenCLImageAlgorithm *gaussian = new OpenCLGaussianImage();
    OpenCLImageAlgorithm *itf = new OpenCLIntToFloat();
    OpenCLImageAlgorithm *fti = new OpenCLFloatToInt();

    cv::Mat gaussian_kernel = cv::getGaussianKernel(25, -1, CV_32F);

    OpenCLGaussianParams params;
    params.setMask(5, gaussian_kernel.data);
    gaussian->setParams(params);

    gaussian_stream.pushAlgorithm(itf);
    gaussian_stream.pushAlgorithm(gaussian);
    gaussian_stream.pushAlgorithm(fti);
    
    OpenCLDevice device = OpenCLDevice::getDevices().front();

    cv::Mat input, input_tmp;
    input = cv::imread("in.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    //input.convertTo(input_tmp, CV_32F, 1.0/255.0f);
    
    cv::Mat output(input.size(), input.type());
    //cv::Mat output(input_tmp.size(), input_tmp.type());
    
    gaussian_stream.setDataSize(input.size().width, input.size().height);
    
    gaussian_stream.setDevice(device);

    gaussian_stream.prepare();

    gaussian_stream.processImage(input.data, output.data);

    //std::cout << output.at<int>(0,0) << "\n";
    std::cout << gaussian_stream.getTime() << "\n";
    //output.convertTo(output, CV_8U, 255.0);
    cv::imwrite("out.bmp", output);
  }
  catch (OpenCLDeviceException ex)
  {
    std::cout << ex.getFullMessage() << "\n" << ex.getSource() << "\n";
  }
  catch (OpenCLException ex)
  {
    std::cout << ex.getFullMessage() << "\n";
  }

  return a;
#endif
}

#endif

