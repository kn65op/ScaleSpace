#pragma once

#include "ScaleSpaceOpenCV.h"

class ScaleSpaceOpenCVCPU : public ScaleSpaceOpenCV
{
public:
  /**
   * Contructor which setting mode (ScaleSpaceMode). 
   */
  ScaleSpaceOpenCVCPU(ScaleSpaceMode mode = ScaleSpaceMode::Pure);

  ~ScaleSpaceOpenCVCPU(void);
};

