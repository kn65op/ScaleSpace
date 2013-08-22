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

void ScaleSpaceOpenCVGPU::calcEdge(cv::Mat& cpu_Lx, cv::Mat& cpu_Ly, cv::Mat& cpu_Lxx, cv::Mat& cpu_Lxy, cv::Mat& cpu_Lyy, cv::Mat& cpu_Lxxx, cv::Mat& cpu_Lxxy, cv::Mat& cpu_Lxyy, cv::Mat& cpu_Lyyy, cv::Mat& cpu_L1, cv::Mat& cpu_L2) const
{
  GpuMat Lx, Ly, Lxx, Lxy, Lyy, Lxxx, Lxxy, Lxyy, Lyyy, L1, L2;
  Lx.upload(cpu_Lx);
  Ly.upload(cpu_Ly);
  Lxx.upload(cpu_Lxx);
  Lxy.upload(cpu_Lxy);
  Lyy.upload(cpu_Lyy);
  Lxxx.upload(cpu_Lxxx);
  Lxxy.upload(cpu_Lxxy);
  Lxyy.upload(cpu_Lxyy);
  Lyyy.upload(cpu_Lyyy);

  GpuMat Lx_sqr, Lx_sqrLxx, LxLy, LxLyLxy, LxLyLxy2, Ly_sqr, Ly_sqrLyy, sumL1;
  
  //L1 = Lx.mul(Lx.mul(Lxx)) + 2 * Lx.mul(Ly.mul(Lxy)) + Ly.mul(Ly.mul(Lyy));
  multiply(Lx, Lx, Lx_sqr);
  multiply(Lx_sqr, Lxx, Lx_sqrLxx);
  multiply(Lx, Ly, LxLy);
  multiply(LxLy, Lxy, LxLyLxy);
  multiply(LxLyLxy, 2, LxLyLxy2);
  multiply(Ly, Ly, Ly_sqr);
  multiply(Ly_sqr, Lyy, Ly_sqrLyy);
  add(Lx_sqrLxx, LxLyLxy2, sumL1);
  add(sumL1, Lx_sqrLxx, L1);

  //setLowValuesToZero(L1);
  L1.download(cpu_L1);

  GpuMat Lx_cube, Lx_cubeLxxx, Lx_sqrLy, Lx_sqrLyLxxy, Lx_sqrLyLxxy3, Ly_sqrLx, Ly_sqrtLxLxyy, Ly_sqrtLxLxyy3, Ly_cube, Ly_cubeLyyy, sumL21, sumL22;
  //L2 = Lx.mul(Lx).mul(Lx).mul(Lxxx) + 
  multiply(Lx_sqr, Lx, Lx_cube);
  multiply(Lx_cube, Lxxx, Lx_cubeLxxx);
  //3 * Lx.mul(Lx).mul(Ly).mul(Lxxy) + 
  multiply(Lx_sqr, Ly, Lx_sqrLy);
  multiply(Lx_sqrLy, Lxxy, Lx_sqrLyLxxy);
  multiply(Lx_sqrLyLxxy, 3, Lx_sqrLyLxxy3);
  //3 * Lx.mul(Ly).mul(Ly).mul(Lxyy) + 
  multiply(Ly_sqr, Lx, Ly_sqrLx);
  multiply(Ly_sqrLx, Lxyy, Ly_sqrtLxLxyy);
  multiply(Ly_sqrtLxLxyy, 3, Ly_sqrtLxLxyy3);
  //Ly.mul(Ly).mul(Ly).mul(Lyyy); 
  multiply(Ly_sqr, Ly, Ly_cube);
  multiply(Ly_cube, Lyyy, Ly_cubeLyyy);

  add(Lx_cubeLxxx, Lx_sqrLyLxxy3, sumL21);
  add(sumL21, Ly_sqrtLxLxyy3, sumL22);
  add(sumL22, Ly_cubeLyyy, L2);

  L2.download(cpu_L2);
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
