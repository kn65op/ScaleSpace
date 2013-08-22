#include "ScaleSpaceOpenCVCPU.h"
#include "ScaleSpaceOpenCVGPU.h"

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

void ScaleSpaceOpenCVCPU::calcBlob(cv::Mat& Lxx, cv::Mat& Lyy, float sigma, cv::Mat& L) const
{
  L = abs(Lxx + Lyy) * sigma;
}

void ScaleSpaceOpenCVCPU::calcCorner(cv::Mat& Lx, cv::Mat& Ly, cv::Mat& Lxx, cv::Mat& Lxy, cv::Mat& Lyy, cv::Mat& k) const
{
  k = abs(Lx.mul(Lx).mul(Lyy) +  Ly.mul(Ly).mul(Lxx) - Lx.mul(Ly.mul(Lxy)) * 2.0);
}

void ScaleSpaceOpenCVCPU::calcEdge(cv::Mat& Lx, cv::Mat& Ly, cv::Mat& Lxx, cv::Mat& Lxy, cv::Mat& Lyy, cv::Mat& Lxxx, cv::Mat& Lxxy, cv::Mat& Lxyy, cv::Mat& Lyyy, cv::Mat& L1, cv::Mat& L2) const
{
  L1 = Lx.mul(Lx.mul(Lxx)) + 2 * Lx.mul(Ly.mul(Lxy)) + Ly.mul(Ly.mul(Lyy));
  L2 = Lx.mul(Lx).mul(Lx).mul(Lxxx) + 3 * Lx.mul(Lx).mul(Ly).mul(Lxxy) + 3 * Lx.mul(Ly).mul(Ly).mul(Lxyy) + Ly.mul(Ly).mul(Ly).mul(Lyyy);
}

void ScaleSpaceOpenCVCPU::calcRidge(cv::Mat& Lx, cv::Mat& Ly, cv::Mat& Lxx, cv::Mat& Lxy, cv::Mat& Lyy, cv::Mat& L1, cv::Mat& L2) const
{
  L1 = Lx.mul(Ly).mul(Lxx - Lyy) - (Lx.mul(Lx) - Ly.mul(Ly)).mul(Lxy);
  L2 = (Ly.mul(Ly) - Lx.mul(Lx)).mul(Lxx - Lyy) - 4 * Lx.mul(Ly).mul(Lxy);
}
