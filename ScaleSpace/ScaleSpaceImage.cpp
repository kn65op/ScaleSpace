#include "ScaleSpaceImage.h"

#include <opencv\highgui.h>


ScaleSpaceImage::ScaleSpaceImage(void)
{
  nr_scales = 0;
}


ScaleSpaceImage::~ScaleSpaceImage(void)
{
}

void ScaleSpaceImage::createImage(unsigned int width, unsigned int height, unsigned int scales)
{
  int *dims =  new int [3];
  dims[0] = scales;
  dims[1] = width;
  dims[2] = height;
  image = cv::Mat(3, dims, CV_8UC1);
  delete [] dims;
  nr_scales = 10;
}

void * ScaleSpaceImage::getDataForScale(unsigned int scale)
{
  if (scale <= nr_scales)
  {
    return image.data + scale * (image.elemSize() * image.cols * image.rows);
  }
  throw ScaleSpaceImageException("Too much scale parameter");
}

void ScaleSpaceImage::setOriginalImage(cv::Mat original_image)
{
  memcpy(image.data, image.data, image.elemSize1() * image.cols * image.rows);
}

void ScaleSpaceImage::show()
{
  cv::imshow("im", image);
  while (cv::waitKey(10));
}