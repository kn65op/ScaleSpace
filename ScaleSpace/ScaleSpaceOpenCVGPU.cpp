#include "ScaleSpaceOpenCVGPU.h"

#include <opencv2\gpu\gpu.hpp>

#include <iostream>

using namespace cv::gpu;

ScaleSpaceOpenCVGPU::ScaleSpaceOpenCVGPU(ScaleSpaceMode mode)
  : ScaleSpaceOpenCV(mode)
{
  std::cout << getCudaEnabledDeviceCount() << "\n";
  //TODO: dummy function for initialize cuda
}


ScaleSpaceOpenCVGPU::~ScaleSpaceOpenCVGPU(void)
{
}

void ScaleSpaceOpenCVGPU::filter2D(cv::Mat& src, cv::Mat& dst, cv::Mat& kernel) const
{
  
}

void ScaleSpaceOpenCVGPU::calcBlob(cv::Mat& Lxx, cv::Mat& Lyy, float sigma, cv::Mat& L) const
{
  
}

void ScaleSpaceOpenCVGPU::calcCorner(cv::Mat& Lx, cv::Mat& Ly, cv::Mat& Lxx, cv::Mat& Lxy, cv::Mat& Lyy, cv::Mat& k) const
{
  
}

void ScaleSpaceOpenCVGPU::calcEdge(cv::Mat& Lx, cv::Mat& Ly, cv::Mat& Lxx, cv::Mat& Lxy, cv::Mat& Lyy, cv::Mat& Lxxx, cv::Mat& Lxxy, cv::Mat& Lxyy, cv::Mat& Lyyy, cv::Mat& L1, cv::Mat& L2) const
{
  
}

void ScaleSpaceOpenCVGPU::calcRidge(cv::Mat& Lx, cv::Mat& Ly, cv::Mat& Lxx, cv::Mat& Lxy, cv::Mat& Lyy, cv::Mat& L1, cv::Mat& L2) const
{
  
}