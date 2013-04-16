#include "ScaleSpace.h"

#include <opencv\cv.h>

#include <OpenCLGaussian.h>
#include <OpenCLIntToFloat.h>
#include <OpenCLFloatToInt.h>

ScaleSpace::ScaleSpace(void)
{
  nr_scales = 0;
  scale_step = 0;
  streams =  streams_t();
  prepared = false;
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
  //streams = streams_t(nr_scales + 1, nullptr);

  OpenCLDevice device = OpenCLDevice::getDevices().front();

  OpenCLImageAlgorithm *itf = new OpenCLIntToFloat();
  OpenCLImageAlgorithm *fti = new OpenCLFloatToInt();
 
  for (unsigned int i=0; i< nr_scales; ++i)
  //for (auto & s : streams)
  {
    OpenCLImageAlgorithm *gaussian = new OpenCLGaussianImage();
    unsigned int n = 5;
    cv::Mat gaussian_kernel = cv::getGaussianKernel(n * n, -1, CV_32F);
    OpenCLGaussianParams params;
    params.setMask(n, gaussian_kernel.data);
    gaussian->setParams(params);

    auto s = new OpenCLAlgorithmsStream();

    s->pushAlgorithm(itf);
    s->pushAlgorithm(gaussian);
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

  output.createImage(input.size().width, input.size().height, nr_scales);
  output.setOriginalImage(input);

  int i = 1;
  for (auto s : streams)
  {
    //TMP
    s->setDataSize(input.size().width, input.size().height);
    s->prepare();
    //END TMP
    s->processImage(input.data, output.getDataForScale(i++));
  }

}