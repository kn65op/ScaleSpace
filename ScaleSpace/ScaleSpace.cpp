#include "ScaleSpace.h"


ScaleSpace::ScaleSpace(void)
{
  nr_scales = 0;
  scale_step = 0;
}


ScaleSpace::~ScaleSpace(void)
{
}

void ScaleSpace::setMaxScale(unsigned int max, unsigned int nr)
{
  if (max == 0) throw ScaleSpaceZeroException("setMaxScale: max cannot be0");
  if (nr == 0) throw ScaleSpaceZeroException("setMaxScale: nr cannot be0");
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

bool ScaleSpace::setScalesRange(unsigned int max, unsigned int step)
{
  return true;
}

unsigned int ScaleSpace::getNrScales()
{
  return nr_scales;
}

unsigned int ScaleSpace::getScaleStep()
{
  return scale_step;
}