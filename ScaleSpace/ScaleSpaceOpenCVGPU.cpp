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
