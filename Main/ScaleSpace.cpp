#include "ScaleSpace.h"


ScaleSpace::ScaleSpace(void)
{
}


ScaleSpace::~ScaleSpace(void)
{
}

void ScaleSpace::setMaxScale(unsigned int max, unsigned int nr)
{
  if ((max - 1) % nr)
  {
    throw ScaleSpaceException("setMaxScale: can't calculate step");
  }
  scale_step = (max - 1) / nr;
  nr_scales = nr;
}

void ScaleSpace::setScaleStep(unsigned int step, unsigned int nr)
{
  
}

void ScaleSpace::setScalesRange(unsigned int max, unsigned int step)
{
  
}
