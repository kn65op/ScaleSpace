#include "ScaleSpaceCUDAKernels.h"

#include <opencv2\core\core.hpp>

__global__ void setLowToZero(cv::gpu::DevMem2Df in, cv::gpu::DevMem2Df out)
{
  int row = threadIdx.y;
  int col = threadIdx.x;
  float input = ((float*)in.ptr(row))[col];
  if (in.ptr(row)[col] < 1e-5)
  {
    ((float*)out.ptr(row))[col] = -3e4f;
  }
  else
  {
    ((float*)out.ptr(row))[col] = 4e4f;
    //out.ptr(row)[col] = in.ptr(row)[col];
  }
}

void __host__ setMatToZero(cv::gpu::GpuMat & in, cv::gpu::GpuMat & out)
{
  setLowToZero<<<in.cols, in.rows>>>(in, out);
}