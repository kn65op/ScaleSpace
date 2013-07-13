#include "ScaleSpaceImage.h"

#include <opencv\highgui.h>

#include <fstream>

ScaleSpaceImage::ScaleSpaceImage(void)
{
  nr_scales = 0;
}


ScaleSpaceImage::~ScaleSpaceImage(void)
{
}

void ScaleSpaceImage::createImage(unsigned int height, unsigned int width, unsigned int scales, int type, unsigned int images)//TODO: temp
{
  image.clear();
  int *dims =  new int [3];
  dims[0] = scales;
  dims[1] = this->width = width;
  dims[2] = this->height = height;
  for (unsigned int i=0; i < images; ++i)
  {
    image.push_back(cv::Mat(3, dims, type));
  }
  this->type = type;
  nr_scales = scales;
  nr_images = images;
  delete [] dims;
}

void * ScaleSpaceImage::getDataForScale(unsigned int scale, unsigned int image_number)
{
  if (!image[image_number].isContinuous())
  {
    throw ScaleSpaceImageException("Data is not continuous");
  }
  if (scale >= nr_scales)
  {
    throw ScaleSpaceImageException("Wrong scale parameter: " + std::to_string(scale) + ". Can be 0 -" + std::to_string(nr_scales - 1));  
  }
  int tmp = scale * (image[image_number].elemSize() * width * height);
  return image[1].data + scale * (image[image_number].elemSize() * width * height);
}

void ScaleSpaceImage::setOriginalImage(cv::Mat oimage)
{
  oimage.copyTo(original_image);
  /*for (unsigned int i = 0; i < nr_images; ++i)
  {
    cv::Mat tmp;
    if (type == CV_32FC1)
    {
      original_image.convertTo(tmp, type, 1.0/256.0);
    }
    else if (type == CV_8UC1)
    {
      original_image.copyTo(tmp);
    }
    else
    {
      throw ScaleSpaceImageException("Not supoorted output image type");
    }
    memcpy(image[i].data, tmp.data, image[i].elemSize1() * width * height);
  }*/
}

void ScaleSpaceImage::show(std::string fn)//TODO: not only one image
{
  static int image_nr = 0;
  cv::Mat tmp(height, width, image[0].type());
  memcpy(tmp.data, getDataForScale(0), image[0].elemSize() * width * height);
  cv::Mat tmp2;
  if (type == CV_32FC1)
  {
    tmp.convertTo(tmp2, CV_8UC1, 256.0);
  }
  else if (type == CV_8UC1)
  {
    tmp2 = tmp;
  }
  else
  {
    throw ScaleSpaceImageException("Not supported output image type");
  }
  cv::imwrite("original.bmp", tmp2);

  for (unsigned int i = 0; i < nr_scales; ++i)
  {
    std::string s;
    s = fn + std::to_string(i) + ".jpg";
    memcpy(tmp.data, getDataForScale(i), image[0].elemSize() * width * height);
    std::ofstream out("data1" + std::to_string(image_nr) +  std::to_string(i) + ".txt");
    out << tmp; //very slow
    out.close();
    //for some algorithms
    cv::Mat tmp3(height, width, image[1].type());
    memcpy(tmp3.data, getDataForScale(i, 1), image[1].elemSize() * width * height);
    std::ofstream out2("data2" + std::to_string(image_nr) +  std::to_string(i) + ".txt");
    out2 << tmp3;
    out2.close();//*/ //very slow
    cv::Mat tmp2;
    if (type == CV_32FC1)
    {
      tmp.convertTo(tmp2, CV_8UC1, 256.0);
    }
    else if (type == CV_8UC1)
    {
      tmp2 = tmp;
    }
    else
    {
      throw ScaleSpaceImageException("Not supported output image type");
    }
    cv::imwrite(s.c_str(), tmp2);
  }
  image_nr++;
}

void ScaleSpaceImage::show(cv::Mat & blobs, std::vector<float> & sigmas)
{
  /*cv::Mat tmp2(height, width, image[0].type()), tmp;
  memcpy(tmp2.data, getDataForScale(0), image[0].elemSize() * width * height);
  tmp2.convertTo(tmp, CV_8UC1, 256.0);*/
  cv::Mat tmp(original_image.clone());
  for (unsigned int i=0; i < height; ++i)
  {
    for (unsigned int j=0; j < width; ++j)
    {
      if (uchar scale = blobs.at<uchar>(i,j))
      {
        cv::circle(tmp, cv::Point2i(j, i), static_cast<int>(sigmas[scale] * 1.412f + 0.5f), cv::Scalar(127), 1);
      }
    }
  }
  cv::imwrite("result.bmp",tmp);
}

unsigned int ScaleSpaceImage::getOneImageSize() const
{
  unsigned int size = original_image.size().height * original_image.size().width;
  if (type == CV_32FC1)
  {
    return size * sizeof(float);
  }
  else if (type == CV_8UC1)
  {
    return size * sizeof(unsigned char);
  }
  throw ScaleSpaceImageException("Not supported output image type");
}
