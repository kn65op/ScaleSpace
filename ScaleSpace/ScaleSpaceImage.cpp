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
  dims[0] = scales + 1;
  dims[1] = this->width = width;
  dims[2] = this->height = height;
  image = cv::Mat(3, dims, CV_8UC1);
  delete [] dims;
  nr_scales = 4;
}

void * ScaleSpaceImage::getDataForScale(unsigned int scale)
{
  if (scale <= nr_scales)
  {
    int tmp = scale * (image.elemSize() * width * height);
    return image.data + scale * (image.elemSize() * width * height);
  }
  throw ScaleSpaceImageException("Wrong scale parameter");
}

void ScaleSpaceImage::setOriginalImage(cv::Mat original_image)
{
  memcpy(image.data, image.data, image.elemSize1() * width * height);
}

void ScaleSpaceImage::show()
{
  cv::Mat tmp(width, height, image.type());
  memcpy(tmp.data, getDataForScale(0), image.elemSize() * width * height);
  cv::imshow("im", tmp);
  while (cv::waitKey(10));
}