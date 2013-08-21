#include "ScaleSpaceOpenCVGPU.h"

#include <opencv2\gpu\gpu.hpp>

#include <iostream>

using namespace cv::gpu;

ScaleSpaceOpenCVGPU::ScaleSpaceOpenCVGPU(ScaleSpaceMode mode)
{
  std::cout << getCudaEnabledDeviceCount() << "\n";
}


ScaleSpaceOpenCVGPU::~ScaleSpaceOpenCVGPU(void)
{
}

void ScaleSpaceOpenCVGPU::prepare(ScaleSpaceSourceImageType si_type, ScaleSpaceOutputType out_type)
{
}

void ScaleSpaceOpenCVGPU::processImage(ScaleSpaceImage & image)
{
}
