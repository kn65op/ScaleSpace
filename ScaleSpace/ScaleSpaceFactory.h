#pragma once

#include "ScaleSpace.h"
#include "ScaleSpaceOpenCL.h"
#include "ScaleSpaceOpenCVCPU.h"
#include "ScaleSpaceOpenCVGPU.h"

class ScaleSpaceFactory
{
public:
  /**
   * Get ScaleSpace implementation. If processor is unavailable (eg. no GPGPU libraries is available) it throws NoPlatformScaleSpaceException.
   * @param processor Procesor used to calculate ScaleSpace.
   * @param mode What objects to detect.
   * @return Pointer to created ScaleSpace implementation.
   */
  static ScaleSpace * getScaleSpace(ScaleSpaceProcessor processor, ScaleSpaceMode mode);
};
