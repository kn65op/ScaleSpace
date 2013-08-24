#include "ScaleSpaceCUDAKernels.h"

#include <opencv2\core\core.hpp>

__global__ void setLowToZero(cv::gpu::PtrStepSzf in, cv::gpu::PtrStepSzf out)
{
  int row = blockIdx.y;
  int col = blockIdx.x;
  float input = ((float*)in.ptr(row))[col];
  if (in.ptr(row)[col] < 1e-5)
  {
    ((float*)out.ptr(row))[col] = -1e4f;
  }
  else
  {
    ((float*)out.ptr(row))[col] = 1e4f;
    //out.ptr(row)[col] = in.ptr(row)[col];
  }
}

void __host__ setMatToZero(cv::gpu::GpuMat & in, cv::gpu::GpuMat & out)
{
  dim3 co(1, 1);  
  dim3 cos(in.cols, in.rows);
  setLowToZero<<<cos, co>>>(in, out);
}