#include "ScaleSpace.h"

#include <opencv2\imgproc\imgproc.hpp>

#include <sstream>

ScaleSpace::ScaleSpace(ScaleSpaceMode mode /*= ScaleSpaceMode::Pure*/)
{
}


ScaleSpace::~ScaleSpace(void)
{
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

  makeSigmas();
}

void ScaleSpace::setScaleStep(unsigned int step, unsigned int nr)
{
  prepared = false;
  if (step == 0) throw ScaleSpaceZeroException("setScaleStep: step cannot be 0");
  if (nr == 0) throw ScaleSpaceZeroException("setScaleStep: nr cannot be 0");
  if (step % 2) throw ScaleSpaceException("setScaleStep: step has to be even");

  scale_step = step;
  nr_scales = nr;

  makeSigmas();
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

  makeSigmas();
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

void ScaleSpace::makeSigmas()
{
  sigmas.clear();
  
  unsigned int scale = 0;
  for (unsigned int i = 0; i < nr_scales; ++i)
  {
    scale = 1 + scale_step * (i + 1);
    // 0.3*((ksize-1)*0.5 - 1) + 0.8 .
    float sigma = 0.3f * ((scale) * 0.5f - 1.0f) + 0.8f;
    sigmas.push_back(sigma);
  }
}

cv::Mat ScaleSpace::getGaussianForScale(unsigned int scale)
{
  if (scale >= nr_scales)
  {
    throw ScaleSpaceException("Wrong scale parameter: " + std::to_string(scale) + ". Can be 0 -" + std::to_string(nr_scales - 1));
  }
  unsigned int gaussian_size = getGaussianSizeForScale(scale);
  cv::Mat gaussian_kernel = cv::getGaussianKernel(gaussian_size, sigmas[scale], CV_32F);
  return gaussian_kernel * gaussian_kernel.t();
}

unsigned int ScaleSpace::getGaussianSizeForScale(unsigned int scale)
{
  return 1 + scale_step * (scale + 1);
}

std::ostream & operator<<(std::ostream& out, ScaleSpaceProcessor processor)
{
  switch(processor)
  {
  case ScaleSpaceProcessor::OPENCL:
    out << "OpenCL";
    break;
  case ScaleSpaceProcessor::OPENCV_CPU:
    out << "OpenCVCPU";
    break;
  case ScaleSpaceProcessor::OPENCV_GPU:
    out << "OpenCVGPU";
    break;
  }

  return out;
}

std::ostream & operator<<(std::ostream& out, ScaleSpaceMode mode)
{
  switch(mode)
  {
  case ScaleSpaceMode::Blobs:
    out << "Blobs second version";
    break;
  case ScaleSpaceMode::Corners:
    out << "Corners";
    break;
  case ScaleSpaceMode::Edges:
    out << "Edges";
    break;
  case ScaleSpaceMode::Laplacian:
    out << "Blobs first version";
    break;
  case ScaleSpaceMode::Pure:
    out << "Only gaussian";
    break;
  case ScaleSpaceMode::Ridges:
    out << "Ridges";
    break;
  }

  return out;
}

std::ostream & operator<<(std::ostream& out, ScaleSpaceSourceImageType type)
{
  switch(type)
  {
  case ScaleSpaceSourceImageType::Bayer:
    out << "Bayer";
    break;
  case ScaleSpaceSourceImageType::Gray:
    out << "Grayscale";
    break;
  }

  return out;
}

std::string getStringFromScaleSpaceProcessor(const ScaleSpaceProcessor & processor)
{
  std::stringstream ss;
  ss << processor;
  return ss.str();
}

void ScaleSpace::setParameters(ScaleSpaceParameters par)
{
  parameters = par;
}
