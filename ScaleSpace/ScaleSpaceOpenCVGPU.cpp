#include "ScaleSpaceOpenCVGPU.h"

#include <opencv2\gpu\gpu.hpp>
#include <opencv2\core\mat.hpp>

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
  GpuMat src_gpu, dst_gpu;
  src_gpu.upload(src);

  cv::gpu::filter2D(src_gpu, dst_gpu, -1, kernel);

  dst_gpu.download(dst);
}

void ScaleSpaceOpenCVGPU::calcBlob(cv::Mat& Lxx, cv::Mat& Lyy, float sigma, cv::Mat& L) const
{
  GpuMat gpu_Lxx, gpu_Lyy, gpu_L, gpu_tmp, sumLxxLyy;
  gpu_Lxx.upload(Lxx);
  gpu_Lyy.upload(Lyy);

  add(gpu_Lxx, gpu_Lyy, gpu_tmp);
  multiply(gpu_tmp, sigma, sumLxxLyy);
  abs(sumLxxLyy, gpu_L);
  
  gpu_L.download(L);
}

void ScaleSpaceOpenCVGPU::calcCorner(cv::Mat& Lx, cv::Mat& Ly, cv::Mat& Lxx, cv::Mat& Lxy, cv::Mat& Lyy, cv::Mat& k) const
{
  GpuMat gpu_Lx, gpu_Ly, gpu_Lxx, gpu_Lxy, gpu_Lyy, gpu_k;
  GpuMat Lx_sqr, Ly_sqr, Lx_sqr_Lyy, Ly_sqr_Lxx, sumLxLy, LxLy, LxLyLxy, LxLyLxy2, difference;
  gpu_Lx.upload(Lx);
  gpu_Ly.upload(Ly);
  gpu_Lxx.upload(Lxx);
  gpu_Lxy.upload(Lxy);
  gpu_Lyy.upload(Lyy);

  //k = abs(Lx.mul(Lx).mul(Lyy) +  Ly.mul(Ly).mul(Lxx) - Lx.mul(Ly.mul(Lxy)) * 2.0);
  sqr(gpu_Lx, Lx_sqr);
  sqr(gpu_Ly, Ly_sqr);

  multiply(Lx_sqr, gpu_Lyy, Lx_sqr_Lyy);
  multiply(Ly_sqr, gpu_Lxx, Ly_sqr_Lxx);

  add(Lx_sqr_Lyy, Ly_sqr_Lxx, sumLxLy);

  multiply(gpu_Lx, gpu_Ly, LxLy);
  multiply(LxLy, gpu_Lxy, LxLyLxy);
  multiply(LxLyLxy, 2, LxLyLxy2);

  subtract(sumLxLy, LxLyLxy2, difference);
  abs(difference, gpu_k);

  gpu_k.download(k);
}

void ScaleSpaceOpenCVGPU::calcEdge(cv::Mat& Lx, cv::Mat& Ly, cv::Mat& Lxx, cv::Mat& Lxy, cv::Mat& Lyy, cv::Mat& Lxxx, cv::Mat& Lxxy, cv::Mat& Lxyy, cv::Mat& Lyyy, cv::Mat& L1, cv::Mat& L2) const
{
  
}

void ScaleSpaceOpenCVGPU::calcRidge(cv::Mat& Lx, cv::Mat& Ly, cv::Mat& Lxx, cv::Mat& Lxy, cv::Mat& Lyy, cv::Mat& L1, cv::Mat& L2) const
{
  
}


void ScaleSpaceOpenCVGPU::calcRidgeMax(cv::Mat& L1, cv::Mat& L2, cv::Mat& out) const
{
  
}

void ScaleSpaceOpenCVGPU::calcEdgeMax(cv::Mat& L1, cv::Mat& L2, cv::Mat& out) const
{
  
}

void ScaleSpaceOpenCVGPU::calcMaxInScale(cv::Mat& L, cv::Mat& out) const
{
  
}
