#include "ScaleSpaceFactory.h"

ScaleSpace * ScaleSpaceFactory::getScaleSpace(ScaleSpaceProcessor processor, ScaleSpaceMode mode)
{
  switch (processor)
  {
  case ScaleSpaceProcessor::OPENCL:
    return new ScaleSpaceOpenCL(mode);
  case ScaleSpaceProcessor::OPENCV_CPU:
    return new ScaleSpaceOpenCVCPU(mode);
  case ScaleSpaceProcessor::OPENCV_GPU:
    return new ScaleSpaceOpenCVGPU(mode);
  default:
    return nullptr;
  }
}
