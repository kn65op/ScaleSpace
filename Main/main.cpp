#include <ScaleSpace.h>
#include "getoptpp\getopt_pp.h"

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

void printHelp();

int main(int argc, char * argv[])
{
  GetOpt::GetOpt_pp opt(argc, argv);
  if (opt >> GetOpt::OptionPresent('h', "help"))
  {
    printHelp();
    return 0;
  }
  std::string in_file;
  if (!(opt >> GetOpt::Option('i', "in", in_file)))
  {
    printHelp();
    return 0;
  }

  cv::Mat input;
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
    input = cv::imread(in_file.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
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
}

#if 0


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

void printHelp()
{
  std::cout << "Help\n";
}