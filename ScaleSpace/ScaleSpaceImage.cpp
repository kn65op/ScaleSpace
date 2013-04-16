#include "ScaleSpaceImage.h"


ScaleSpaceImage::ScaleSpaceImage(void)
{
  nr_scales = 0;
}


ScaleSpaceImage::~ScaleSpaceImage(void)
{
}

void ScaleSpaceImage::createImage(unsigned int width, unsigned int height, unsigned int scales)
{
  
}

void * ScaleSpaceImage::getDataForScale(unsigned int scale)
{
  if (scale < nr_scales)
  {
    return image.data + scale * (image.elemSize() * image.cols * image.rows);
  }
  throw ScaleSpaceImageException("Too much scale parameter");
}

void ScaleSpaceImage::setNumberOfScales(unsigned int scales)
{
  nr_scales = scales;
}

void ScaleSpaceImage::setOriginalImage(cv::Mat image)
{
  
}