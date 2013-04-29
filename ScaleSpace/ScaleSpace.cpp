#include "ScaleSpace.h"

#include <opencv\cv.h>

#include <OpenCLGaussian.h>
#include <OpenCLIntToFloat.h>
#include <OpenCLFloatToInt.h>
#include <OpenCLLaplacian.h>


#define DEBUG_SS

#ifdef DEBUG_SS
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
    //throw ScaleSpaceZeroException("prepare: There is no scales to prepare");
  }

  OpenCLDevice device = OpenCLDevice::getDevices().front();

 
  for (unsigned int i=0; i< nr_scales; ++i)
  {
    OpenCLImageAlgorithm *itf = new OpenCLIntToFloat();
    OpenCLImageAlgorithm *fti = new OpenCLFloatToInt();
    OpenCLImageAlgorithm *gaussian = new OpenCLGaussianImage();
    unsigned int scale = 1 + scale_step * (i + 1);

    #ifdef DEBUG_SS
    std::cout << "Scale step: " << scale << "\n";
    #endif

    cv::Mat gaussian_kernel = cv::getGaussianKernel(scale, -1, CV_32F);
    cv::Mat gaussian_kernel_2d = gaussian_kernel * gaussian_kernel.t();
    OpenCLGaussianParams params;
    params.setMask(scale, gaussian_kernel_2d.data);
    gaussian->setParams(params);

    auto s = new OpenCLAlgorithmsStream();

    s->pushAlgorithm(itf);
    s->pushAlgorithm(gaussian);

    OpenCLImageAlgorithm *laplacian = nullptr;
    switch (calc_mode)
    {
    case ScaleSpaceMode::Laplacian:
      laplacian = new OpenCLLaplacian();
      s->pushAlgorithm(laplacian);
      break;
    default: //and Pure
      break;
    }

    s->pushAlgorithm(fti);
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
    output.createImage(input.size().height, input.size().width, nr_scales);
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
  }

  output.setOriginalImage(input);
  cv::Mat tmp(input.size().height, input.size().width, CV_8UC1);
  int i = 1;
  for (auto s : streams)
  {
    
    #ifdef DEBUG_SS
    std::cout << "processing: " << i << " - ";
    #endif
    s->processImage(input.data, output.getDataForScale(i++));
    #ifdef DEBUG_SS
    std::cout << s->getTime() << "\n";
    #endif
  }

}