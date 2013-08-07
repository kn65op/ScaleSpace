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

  makeSigmas();
}

void ScaleSpace::setScaleStep(unsigned int step, unsigned int nr)
{
  prepared = false;
  if (step == 0) throw ScaleSpaceZeroException("setScaleStep: step cannot be 0");
  if (nr == 0) throw ScaleSpaceZeroException("setScaleStep: nr cannot be 0");
  if (step % 2) throw ScaleSpaceException("setScaleStep: step has to be even");

  scale_step = step;
  nr_scales = nr;

  makeSigmas();
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

  makeSigmas();
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

void ScaleSpace::makeSigmas()
{
  sigmas.clear();
  
  unsigned int scale = 0;
  for (unsigned int i = 0; i < nr_scales; ++i)
  {
    scale = 1 + scale_step * (i + 1);
    // 0.3*((ksize-1)*0.5 - 1) + 0.8 .
    float sigma = 0.3f * ((scale) * 0.5f - 1.0f) + 0.8f;
    sigmas.push_back(sigma);
  }
}