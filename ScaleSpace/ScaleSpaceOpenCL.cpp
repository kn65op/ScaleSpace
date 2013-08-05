#include "ScaleSpaceOpenCL.h"

#include <opencv\cv.h>

#include <OpenCLGaussian.h>
#include <OpenCLIntToFloat.h>
#include <OpenCLFloatToInt.h>
#include <OpenCLLaplacian.h>
#include <OpenCLEdgeDetector.h>
#include <OpenCLRidgeDetector.h>
#include <OpenCLCornerDetector.h>
#include <OpenCLBlobDetector.h>
#include <OpenCLFindMaxin3DImage.h>
#include <OpenCLFindEdgesIn3DImage.h>
#include <OpenCLFindRidgesIn3DImage.h>
#include <OpenCLFindMaxin2DImage.h>
#include <OpenCLFindEdgesIn2DImage.h>
#include <OpenCLFindRidgesIn2DImage.h>
#include <OpenCLBayerFilter.h>
#include <OpenCLRGBToGray.h>

#define NOTICE_SS
//#define DEBUG_SS
#define INFO_SS

#ifdef NOTICE_SS
#include <opencv\highgui.h>
#include <iostream>
#endif
#ifdef DEBUG_SS
#include <fstream>
#endif
   
ScaleSpaceOpenCL::ScaleSpaceOpenCL(ScaleSpaceMode mode /* = Pure */)
{
  nr_scales = 0;
  scale_step = 0;
  streams =  streams_t();
  prepared = false;

  last_height = last_width = last_scale = 0;

  calc_mode = mode;

  post_processing = nullptr;
}

ScaleSpaceOpenCL::~ScaleSpaceOpenCL(void)
{
  clearStreams();
}


void ScaleSpaceOpenCL::clearStreams()
{
  for (auto s : streams)
  {
    delete s;
  }
  streams.clear();
}

void ScaleSpaceOpenCL::prepare(ScaleSpaceSourceImageType si_type, ScaleSpaceOutputType out_type)
{
  output_type = out_type;
  if (!streams.empty())
  {
    clearStreams();
    sigmas.clear();
    //throw ScaleSpaceZeroException("prepare: There is no scales to prepare");
  }

  OpenCLDevice device = OpenCLDevice::getDevices().front();
 
  for (unsigned int i=0; i< nr_scales; ++i)
  {
    auto s = new OpenCLAlgorithmsStream();
    
    OpenCL2DTo2DImageAlgorithmForStream *itf = new OpenCLIntToFloat();
    s->pushAlgorithm(itf);

    if (si_type == ScaleSpaceSourceImageType::Bayer)
    {
      OpenCL2DTo2DImageAlgorithmForStream *bayer = new OpenCLBayerFilterForStream();
      OpenCLBayerFilterParams bayer_params;
      bayer->setParams(bayer_params);
      s->pushAlgorithm(bayer);

      OpenCL2DTo2DImageAlgorithmForStream *rgb2gray = new OpenCLRGBToGray();
      s->pushAlgorithm(rgb2gray);
    }

    OpenCL2DTo2DImageAlgorithmForStream *gaussian = new OpenCLGaussianImage();
    unsigned int scale = 1 + scale_step * (i + 1);

    #ifdef INFO_SS
    std::cout << "Scale: " << scale << "\n";
    #endif

    // 0.3*((ksize-1)*0.5 - 1) + 0.8 .
    float sigma = 0.3f * ((scale) * 0.5f - 1.0f) + 0.8f;
    sigmas.push_back(sigma);

    cv::Mat gaussian_kernel = cv::getGaussianKernel(scale, sigma, CV_32F);
    cv::Mat gaussian_kernel_2d = gaussian_kernel * gaussian_kernel.t();
    #ifdef DEBUG_SS_GAUSSIAN
    for (unsigned int i = 0; i < scale; ++i)
    {
      for (unsigned int j = 0; j < scale; ++j)
      {
        std::cout << gaussian_kernel_2d.at<float>(i,j) << " ";
      }
      std::cout << "\n";
    }
    #endif
    OpenCLGaussianParams params;
    params.setMask(scale, gaussian_kernel_2d.data);
    gaussian->setParams(params);
    s->pushAlgorithm(gaussian);

    OpenCL2DTo2DImageAlgorithmForStream *recognizer = nullptr;
    OpenCLLaplacianParams laplacian_params;
    laplacian_params.setSigma(sigma*sigma);
    switch (calc_mode)
    {
    case ScaleSpaceMode::Laplacian:
      recognizer = new OpenCLLaplacian();
      recognizer->setParams(laplacian_params);
      s->pushAlgorithm(recognizer);
      type = CV_32FC1;
      #ifdef INFO_SS
      std::cout << "Mode: laplacian\n";
      #endif
      switch (out_type)
      {
      case ScaleSpaceOutputType::ONE_IMAGE:
        post_processing = new OpenCLFindMaxin3DImage();
        break;
      case ScaleSpaceOutputType::IMAGE_FOR_SCALE:
        post_processing = new OpenCLFindMaxin2DImage();
        break;
      }
      break;
    case ScaleSpaceMode::Edges:
      recognizer = new OpenCLEdgeDetector();
      s->pushAlgorithm(recognizer);
      type = CV_32FC1;
      #ifdef INFO_SS
      std::cout << "Mode: edges\n";
      #endif
      switch (out_type)
      {
      case ScaleSpaceOutputType::ONE_IMAGE:
        post_processing = new OpenCLFindEdgesIn3DImage();
        break;
      case ScaleSpaceOutputType::IMAGE_FOR_SCALE:
        post_processing = new OpenCLFindEdgesIn2DImage();
        break;
      }
      break;
    case ScaleSpaceMode::Blobs:
      recognizer = new OpenCLBlobDetector();
      s->pushAlgorithm(recognizer);
      type = CV_32FC1;
      #ifdef INFO_SS
      std::cout << "Mode: blobs\n";
      #endif
      switch (out_type)
      {
      case ScaleSpaceOutputType::ONE_IMAGE:
        post_processing = new OpenCLFindMaxin3DImage();
        break;
      case ScaleSpaceOutputType::IMAGE_FOR_SCALE:
        post_processing = new OpenCLFindMaxin2DImage();
        break;
      }
      break;
    case ScaleSpaceMode::Corners:
      recognizer = new OpenCLCornerDetector();
      s->pushAlgorithm(recognizer);
      type = CV_32FC1;
      #ifdef INFO_SS
      std::cout << "Mode: corners\n";
      #endif
      switch (out_type)
      {
      case ScaleSpaceOutputType::ONE_IMAGE:
        post_processing = new OpenCLFindMaxin3DImage();
        break;
      case ScaleSpaceOutputType::IMAGE_FOR_SCALE:
        post_processing = new OpenCLFindMaxin2DImage();
        break;
      }
      break;
    case ScaleSpaceMode::Ridges:
      recognizer = new OpenCLRidgeDetector();
      s->pushAlgorithm(recognizer);
      type = CV_32FC1;
      #ifdef INFO_SS
      std::cout << "Mode: ridges\n";
      #endif
      switch (out_type)
      {
      case ScaleSpaceOutputType::ONE_IMAGE:
        post_processing = new OpenCLFindRidgesIn3DImage();
        break;
      case ScaleSpaceOutputType::IMAGE_FOR_SCALE:
        post_processing = new OpenCLFindRidgesIn2DImage();
        break;
      }
      break;
    default: //and Pure
      //throw ScaleSpaceException("Pure not working for now");
      recognizer = new OpenCLFloatToInt();
      s->pushAlgorithm(recognizer);
      type = CV_8UC1;
      break;
    }
    
    if(post_processing)
    {
      post_processing->setDevice(device);
    }
    s->setDevice(device);

    streams.push_back(s);
  }

  prepared = true;
}

void ScaleSpaceOpenCL::processImage(cv::Mat& input, ScaleSpaceImage& output)
{
  if (!prepared)
  {
    std::cout << "Algorithm preparation using grayscale input";
    prepare(ScaleSpaceSourceImageType::Gray);
  }

  if (last_height != input.size().height || last_width != input.size().width || last_scale != nr_scales)
  {
    unsigned int images = 1;
    switch (calc_mode)
    {
    case ScaleSpaceMode::Edges:
      images = 2;
      break;
    case ScaleSpaceMode::Ridges:
      images = 2;
      break;
    }
    output.createImage(input.size().height, input.size().width, nr_scales, type, images);
    for (auto s : streams)
    {
      s->setDataSize(input.size().width, input.size().height); //not like OpenCV
      s->prepare();
    }
    last_height = input.size().height;
    last_width = input.size().width;
    last_scale = nr_scales;
    #ifdef INFO_SS
    std::cout << "NEW\n";
    #endif
    if (post_processing)
    {
      post_processing->setDataSize(input.size().width, input.size().height, nr_scales);
      post_processing->prepare();
      if (output_type == ScaleSpaceOutputType::ONE_IMAGE)
      {
        if (calc_mode == ScaleSpaceMode::Edges)  //TODO: fix 
        {
          OpenCLFindEdgesIn3DImageParams edges_params;
          edges_params.setData(output.getDataForScale(0,1));
          post_processing->setParams(edges_params);
        }
        if (calc_mode == ScaleSpaceMode::Ridges) //TODO: fix
        {
          OpenCLFindRidgesIn3DImageParams ridge_params;
          ridge_params.setData(output.getDataForScale(0,1));
          post_processing->setParams(ridge_params);
        }
      }
    }
  }

  output.setInput(input);
  int i = 0;
  for (auto s : streams)
  {
    #ifdef INFO_SS
    std::cout << "processing: " << i << " - ";
    #endif
    s->processImage(input.data, output.getDataForScale(i));
    void * additional_output = s->getLastAlgorithmAdditionalOutput();
    #ifdef DEBUG_SS
    std::cout << "additional_output = " << additional_output << "\n";
    #endif
    if (additional_output)
    {
      #ifdef INFO_SS
      std::cout << "Storing additional output\n";
      #endif
      memcpy(output.getDataForScale(i, 1), additional_output, output.getOneImageSize());
    }
    #ifdef NOTICE_SS
    std::cout << s->getTime() << "\n";
    //std::cout << (int)(*((unsigned char*)output.getDataForScale(i - 1))) << "\n";
    #endif
    ++i;
  }
  output.show("before"); 
  cv::Mat outp = cv::Mat::zeros(input.size(), input.type());
  if (post_processing)
  {
    #ifdef INFO_SS
    std::cout << "Post processing\n";
    #endif
    if (output_type == ScaleSpaceOutputType::ONE_IMAGE)
    {
      #ifdef INFO_SS
      std::cout << "One image output\n";
      #endif
      post_processing->processData(output.getDataForScale(0), outp.data);
    }
    else if (output_type == ScaleSpaceOutputType::IMAGE_FOR_SCALE)
    {
      #ifdef INFO_SS
      std::cout << "Image for scale output\n";
      #endif
      for (unsigned int i = 0; i < nr_scales ; ++i)
      {
        if (calc_mode == ScaleSpaceMode::Edges || calc_mode == ScaleSpaceMode::Ridges)
        {
          OpenCLFindEdgesIn2DImageParams params;
          params.setData(output.getDataForScale(i,1));
          post_processing->setParams(params);
        }

        post_processing->prepare();
        post_processing->processData(output.getDataForScale(i), output.getDataForOutput(i));
        cv::imwrite("outp" + std::to_string(i) + ".bmp", output.getOutput(i));
      }
    }
  }
#ifdef DEBUG_SS
  std::ofstream ostr("outp.txt");
#endif
  switch (calc_mode)
  {
  case ScaleSpaceMode::Laplacian:
   // output.show(outp, sigmas); 
    outp = outp * 255 / nr_scales;
    break;
  case ScaleSpaceMode::Edges:
    break;
  case ScaleSpaceMode::Blobs:
  //  output.show(outp, sigmas); 
    outp = outp * 255 / nr_scales;
    break;
  case ScaleSpaceMode::Corners:
#ifdef DEBUG_SS
    ostr << outp; //very slow
    ostr.close();
#endif
    outp = outp * 255 / nr_scales;
    break;
  case ScaleSpaceMode::Ridges:
    break;
  default: //and Pure
    break;
  }

  cv::imwrite("outp.bmp", outp);//*/
  //output /= nr_scales;
  //output.show("after");
}
