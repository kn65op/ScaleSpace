#include "ScaleSpace.h"


ScaleSpace::ScaleSpace(ScaleSpaceMode mode /*= ScaleSpaceMode::Pure*/)
{
}


ScaleSpace::~ScaleSpace(void)
{
}

void ScaleSpace::setMaxScale(unsigned int max, unsigned int nr)
{
  prepared = false;
  if (max == 0) throw ScaleSpaceZeroException("setMaxScale: max cannot be 0");
  if (nr == 0) throw ScaleSpaceZeroException("setMaxScale: nr cannot be 0");
  if ((max - 1) % nr)
  {
    throw ScaleSpaceException("setMaxScale: can't calculate step");
  }
  unsigned int step = (max - 1) / nr;
  if (step % 2)
  {
    throw ScaleSpaceException("setMaxScale: step is not odd");
  }
  scale_step = step;
  nr_scales = nr;
}

void ScaleSpace::setScaleStep(unsigned int step, unsigned int nr)
{
  prepared = false;
  if (step == 0) throw ScaleSpaceZeroException("setScaleStep: step cannot be 0");
  if (nr == 0) throw ScaleSpaceZeroException("setScaleStep: nr cannot be 0");
  if (step % 2) throw ScaleSpaceException("setScaleStep: step has to be even");

  scale_step = step;
  nr_scales = nr;
}

bool ScaleSpace::setScalesRange(unsigned int max, unsigned int step)
{
  prepared = false;
  if (max == 0) throw ScaleSpaceZeroException("setScalesRange: max cannot be 0");
  if (step == 0) throw ScaleSpaceZeroException("setScalesRange: step cannot be 0");
  if (step % 2) throw ScaleSpaceException("setScalesRange: step has to be even");

  bool ret = true;
  if ((max - 1) % step)
  {
    ret = false;
  }

  nr_scales = (max - 1) / step;
  scale_step = step;
  return ret;
}


unsigned int ScaleSpace::getNrScales()
{
  return nr_scales;
}

unsigned int ScaleSpace::getScaleStep()
{
  return scale_step;
}

