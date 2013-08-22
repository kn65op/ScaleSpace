#include "ScaleSpaceOpenCVCPU.h"

#include <opencv2\imgproc\imgproc.hpp>

ScaleSpaceOpenCVCPU::ScaleSpaceOpenCVCPU(ScaleSpaceMode mode)
  : ScaleSpaceOpenCV(mode)
{
}


ScaleSpaceOpenCVCPU::~ScaleSpaceOpenCVCPU(void)
{
}

void ScaleSpaceOpenCVCPU::filter2D(cv::Mat& src, cv::Mat& dst, cv::Mat& kernel) const
{
  cv::filter2D(src, dst, -1, kernel);
}
