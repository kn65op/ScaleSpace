#include "ScaleSpace.h"

#include <opencv\cv.h>

#include <OpenCLGaussian.h>
#include <OpenCLIntToFloat.h>
#include <OpenCLFloatToInt.h>
#include <OpenCLLaplacian.h>
#include <OpenCLEdgeDetector.h>
#include <OpenCLCornerDetector.h>
#include <OpenCLBlobDetector.h>

#define DEBUG_SS

#ifdef DEBUG_SS
#include <opencv\highgui.h>
#include <iostream>
#endif
   
ScaleSpace::ScaleSpace(ScaleSpaceMode mode /* = Pure */)
{
  nr_scales = 0;
  scale_step = 0;
  streams =  streams_t();
  prepared = false;

  last_height = last_width = last_scale = 0;

  calc_mode = mode;
}

ScaleSpace::~ScaleSpace(void)
{
  clearStreams();
}

void ScaleSpace::setMaxScale(unsigned int max, unsigned int nr)
{
  prepared = false;
  if (max == 0) throw ScaleSpaceZeroException("setMaxScale: max cannot be 0");
  if (nr == 0) throw ScaleSpaceZeroException("setMaxScale: nr cannot be 0");
  if ((max - 1) % nr)
  {
    throw ScaleSpaceException("setMaxScale: can't calculate step");
  }
  unsigned int step = (max - 1) / nr;
  if (step % 2)
  {
    throw ScaleSpaceException("setMaxScale: step is not odd");
  }
  scale_step = step;
  nr_scales = nr;
}

void ScaleSpace::setScaleStep(unsigned int step, unsigned int nr)
{
  prepared = false;
  if (step == 0) throw ScaleSpaceZeroException("setScaleStep: step cannot be 0");
  if (nr == 0) throw ScaleSpaceZeroException("setScaleStep: nr cannot be 0");
  if (step % 2) throw ScaleSpaceException("setScaleStep: step has to be even");

  scale_step = step;
  nr_scales = nr;
}

bool ScaleSpace::setScalesRange(unsigned int max, unsigned int step)
{
  prepared = false;
  if (max == 0) throw ScaleSpaceZeroException("setScalesRange: max cannot be 0");
  if (step == 0) throw ScaleSpaceZeroException("setScalesRange: step cannot be 0");
  if (step % 2) throw ScaleSpaceException("setScalesRange: step has to be even");

  bool ret = true;
  if ((max - 1) % step)
  {
    ret = false;
  }

  nr_scales = (max - 1) / step;
  scale_step = step;
  return ret;
}

unsigned int ScaleSpace::getNrScales()
{
  return nr_scales;
}

unsigned int ScaleSpace::getScaleStep()
{
  return scale_step;
}

void ScaleSpace::clearStreams()
{
  for (auto s : streams)
  {
    delete s;
  }
  streams.clear();
}

void ScaleSpace::prepare()
{
  if (!streams.empty())
  {
    clearStreams();
    sigmas.clear();
    //throw ScaleSpaceZeroException("prepare: There is no scales to prepare");
  }

  OpenCLDevice device = OpenCLDevice::getDevices().front();
  fmi3Dimage.setDevice(device);
 
  for (unsigned int i=0; i< nr_scales; ++i)
  {
    OpenCL2DTo2DImageAlgorithmForStream *itf = new OpenCLIntToFloat();
    OpenCL2DTo2DImageAlgorithmForStream *gaussian = new OpenCLGaussianImage();
    unsigned int scale = 1 + scale_step * (i + 1);

    #ifdef DEBUG_SS
    std::cout << "Scale step: " << scale << "\n";
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

    auto s = new OpenCLAlgorithmsStream();

    s->pushAlgorithm(itf);
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
      #ifdef DEBUG_SS
      std::cout << "Mode: laplacian\n";
      #endif
      break;
    case ScaleSpaceMode::Edges:
      recognizer = new OpenCLEdgeDetector();
      s->pushAlgorithm(recognizer);
      type = CV_32FC1;
      #ifdef DEBUG_SS
      std::cout << "Mode: edges\n";
      #endif
      break;
    case ScaleSpaceMode::Blobs:
      recognizer = new OpenCLBlobDetector();
      s->pushAlgorithm(recognizer);
      type = CV_32FC1;
      #ifdef DEBUG_SS
      std::cout << "Mode: blobs\n";
      #endif
      break;
    case ScaleSpaceMode::Corners:
      recognizer = new OpenCLCornerDetector();
      s->pushAlgorithm(recognizer);
      type = CV_32FC1;
      #ifdef DEBUG_SS
      std::cout << "Mode: corners\n";
      #endif
      break;
    default: //and Pure
      //throw ScaleSpaceException("Pure not working for now");
      recognizer = new OpenCLFloatToInt();
      s->pushAlgorithm(recognizer);
      type = CV_8UC1;
      break;
    }

    s->setDevice(device);

    streams.push_back(s);
  }

  prepared = true;
}

void ScaleSpace::processImage(cv::Mat& input, ScaleSpaceImage& output)
{
  if (!prepared)
  {
    prepare();
  }

  if (last_height != input.size().height || last_width != input.size().width || last_scale != nr_scales)
  {
    unsigned int images = 1;
    switch (calc_mode)
    {
    case ScaleSpaceMode::Edges:
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
    #ifdef DEBUG_SS
    std::cout << "NEW\n";
    #endif
    fmi3Dimage.setDataSize(input.size().width, input.size().height, nr_scales);
    fmi3Dimage.prepare();
  }

  output.setOriginalImage(input);
  cv::Mat tmp(input.size().height, input.size().width, CV_8UC1);
  int i = 0;
  for (auto s : streams)
  {
    #ifdef DEBUG_SS
    std::cout << "processing: " << i << " - ";
    #endif
    s->processImage(input.data, output.getDataForScale(++i)); //not copy original image data
    void * additional_output = s->getLastAlgorithmAdditionalOutput();
    if (additional_output)
    {
      memcpy(output.getDataForScale(i, 1), additional_output, output.getOneImageSize());
    }
    #ifdef DEBUG_SS
    std::cout << s->getTime() << "\n";
    //std::cout << (int)(*((unsigned char*)output.getDataForScale(i - 1))) << "\n";
    #endif
  }
  output.show("before"); 
  cv::Mat outp = cv::Mat::zeros(input.size(), input.type());
  fmi3Dimage.processData(output.getDataForScale(1), outp.data); //TODO: remove from non laplacian
  output.show(outp, sigmas); 
  outp = outp * 255 / nr_scales;
  cv::imwrite("outp.bmp", outp);//*/
  //output /= nr_scales;
  //output.show("after");
}

void ScaleSpace::findMax()
{
}