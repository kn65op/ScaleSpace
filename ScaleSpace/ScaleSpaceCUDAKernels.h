#include <opencv2\gpu\gpumat.hpp>

#include <cuda.h>
#include <cuda_runtime.h>
#include <npp.h>

void __host__ setMatToZero(cv::gpu::GpuMat & in, cv::gpu::GpuMat & out);