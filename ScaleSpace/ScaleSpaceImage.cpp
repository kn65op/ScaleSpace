#include "ScaleSpaceImage.h"

#include <opencv\highgui.h>


ScaleSpaceImage::ScaleSpaceImage(void)
{
  nr_scales = 0;
}


ScaleSpaceImage::~ScaleSpaceImage(void)
{
}

void ScaleSpaceImage::createImage(unsigned int height, unsigned int width, unsigned int scales)
{
  int *dims =  new int [3];
  dims[0] = scales + 1;
  dims[1] = this->width = width;
  dims[2] = this->height = height;
  image = cv::Mat(3, dims, CV_8UC1);
  delete [] dims;
  nr_scales = scales;
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
  memcpy(image.data, original_image.data, image.elemSize1() * width * height);
}

void ScaleSpaceImage::show()
{
  cv::Mat tmp(height, width, image.type());
  memcpy(tmp.data, getDataForScale(0), image.elemSize() * width * height);
  cv::imwrite("original.bmp", tmp);
  for (unsigned int i = 1; i < nr_scales; ++i)
  {
    std::string s;
    s = "Scale" + std::to_string(i) + ".bmp";
    memcpy(tmp.data, getDataForScale(i), image.elemSize() * width * height);
    cv::imwrite(s.c_str(), tmp);
  }
}