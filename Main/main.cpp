#if 0

#include <opencv2\legacy\compat.hpp>
#include <opencv2\highgui\highgui_c.h>
#include <opencv2\imgproc\imgproc_c.h>
int main(int argc, char** argv)
{
    const char* filename = argc > 1 ? argv[1] : "lena.jpg";
    IplImage* img = cvLoadImage( filename, 0 ), *cimg;
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* keypoints = 0;
    int i;

    if( !img )
        return 0;
    cvNamedWindow( "image", 1 );
    cvShowImage( "image", img );
    cvNamedWindow( "features", 1 );
    cimg = cvCreateImage( cvGetSize(img), 8, 3 );
    cvCvtColor( img, cimg, CV_GRAY2BGR );

    keypoints = cvGetStarKeypoints( img, storage, cvStarDetectorParams(45) );

    for( i = 0; i < (keypoints ? keypoints->total : 0); i++ )
    {
        CvStarKeypoint kpt = *(CvStarKeypoint*)cvGetSeqElem(keypoints, i);
        int r = kpt.size/2;
        cvCircle( cimg, kpt.pt, r, CV_RGB(0,255,0));
        cvLine( cimg, cvPoint(kpt.pt.x + r, kpt.pt.y + r),
            cvPoint(kpt.pt.x - r, kpt.pt.y - r), CV_RGB(0,255,0));
        cvLine( cimg, cvPoint(kpt.pt.x - r, kpt.pt.y + r),
            cvPoint(kpt.pt.x + r, kpt.pt.y - r), CV_RGB(0,255,0));
    }
    cvShowImage( "features", cimg );
    cvWaitKey();

}

#endif

#if 1

#include <ScaleSpaceFactory.h>
#include <Camera.h>
#include <CameraException.h>
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

//#define DEVICE_INFO
#ifdef DEVICE_INFO

int main(int argc, char * argv[])
{
  std::cout << "SADASD\n";
  OpenCLDevice dev = OpenCLDevice::getDevices().front();
  size_t maxw, maxh, maxd;
  dev.getDeviceInfo(CL_DEVICE_IMAGE3D_MAX_DEPTH, &maxd);
  dev.getDeviceInfo(CL_DEVICE_IMAGE2D_MAX_HEIGHT, &maxh);
  dev.getDeviceInfo(CL_DEVICE_IMAGE2D_MAX_WIDTH, &maxw);

  std::cout << maxw << " " << maxh << " " << maxd << "\n";
}

#else

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

#endif

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
    ScaleSpaceOpenCL ss/*;//*/(ScaleSpaceMode::Laplacian);
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

    output.show();
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


#endif