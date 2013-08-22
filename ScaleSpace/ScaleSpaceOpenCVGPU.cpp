#include "ScaleSpaceOpenCVGPU.h"

#include <opencv2\gpu\gpu.hpp>

#include <iostream>

using namespace cv::gpu;

ScaleSpaceOpenCVGPU::ScaleSpaceOpenCVGPU(ScaleSpaceMode mode)
  : ScaleSpaceOpenCV(mode)
{
  std::cout << getCudaEnabledDeviceCount() << "\n";
}


ScaleSpaceOpenCVGPU::~ScaleSpaceOpenCVGPU(void)
{
}

void ScaleSpaceOpenCVGPU::filter2D(cv::Mat& src, cv::Mat& dst, cv::Mat& kernel) const
{
  
}
