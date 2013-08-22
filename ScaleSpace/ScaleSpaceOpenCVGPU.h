#pragma once
#include "ScaleSpaceOpenCV.h"

class ScaleSpaceOpenCVGPU : public ScaleSpaceOpenCV
{
public:
  /**
   * Contructor which setting mode (ScaleSpaceMode). 
   */
  ScaleSpaceOpenCVGPU(ScaleSpaceMode mode = ScaleSpaceMode::Pure);

  ~ScaleSpaceOpenCVGPU(void);

};

