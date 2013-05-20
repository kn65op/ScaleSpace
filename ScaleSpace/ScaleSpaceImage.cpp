#include "ScaleSpaceImage.h"

#include <opencv\highgui.h>


ScaleSpaceImage::ScaleSpaceImage(void)
{
  nr_scales = 0;
}


ScaleSpaceImage::~ScaleSpaceImage(void)
{
}

void ScaleSpaceImage::createImage(unsigned int height, unsigned int width, unsigned int scales, int type)//TODO: temp
{
  int *dims =  new int [3];
  dims[0] = scales + 1;
  dims[1] = this->width = width;
  dims[2] = this->height = height;
  image = cv::Mat(3, dims, type);
  this->type = type;
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
  cv::Mat tmp;
  original_image.convertTo(tmp, type, 1.0/256.0);
  memcpy(image.data, tmp.data, image.elemSize1() * width * height);
}

void ScaleSpaceImage::show(std::string fn)
{
  static int image_nr = 0;
  cv::Mat tmp(height, width, image.type());
  memcpy(tmp.data, getDataForScale(0), image.elemSize() * width * height);
  cv::Mat tmp2;
  tmp.convertTo(tmp2, CV_8UC1, 256.0);
  cv::imwrite("original.bmp", tmp2);

  for (unsigned int i = 1; i < nr_scales + 1; ++i)
  {
    std::string s;
    s = fn + std::to_string(i) + ".jpg";
    memcpy(tmp.data, getDataForScale(i), image.elemSize() * width * height);
    cv::Mat tmp2;
    tmp.convertTo(tmp2, CV_8UC1, 256.0);
    cv::imwrite(s.c_str(), tmp2);
  }
  image_nr++;
}

void ScaleSpaceImage::show(cv::Mat & blobs, std::vector<float> & sigmas)
{
  cv::Mat tmp2(height, width, image.type()), tmp;
  memcpy(tmp2.data, getDataForScale(0), image.elemSize() * width * height);
  tmp2.convertTo(tmp, CV_8UC1, 256.0);
  for (int i=0; i < height; ++i)
  {
    for (int j=0; j < width; ++j)
    {
      if (uchar scale = blobs.at<uchar>(i,j))
      {
        cv::circle(tmp, cv::Point2i(j, i), sigmas[scale] * 1.412f, cv::Scalar(127), 1);
      }
    }
  }
  cv::imwrite("result.bmp",tmp);
}