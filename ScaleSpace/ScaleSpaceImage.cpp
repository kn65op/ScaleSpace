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

void ScaleSpaceImage::createImage(unsigned int scales, int type, unsigned int images)//TODO: temp
{
  scale_space_images.clear();
  int *dims =  new int [2];
  dims[0] = input.size().width;
  dims[1] = input.size().height;
  for (unsigned int i=0; i < images; ++i)
  {
    vector_mat_t scale_vec;
    for (unsigned int j = 0; j < scales; ++j)
    {
      scale_vec.push_back(cv::Mat(2, dims, type));
    }
    scale_space_images.push_back(scale_vec);
  }
  this->temporary_image_type = type;
  nr_scales = scales;
  nr_images = images;

  //output 
  output.clear();
  dims[0] = input.size().height;
  dims[1] = input.size().width;
  for (unsigned int j = 0; j < scales; ++j)
  {
    output.push_back(cv::Mat(2, dims, CV_8UC1));
  }
  
  delete [] dims;
}

void * ScaleSpaceImage::getDataForScale(unsigned int scale, unsigned int image_number)
{
  return getImageForScale(scale, image_number).data;
}

void ScaleSpaceImage::setInput(cv::Mat image)
{
  image.copyTo(input);
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
    memcpy(image[i].data, tmp.data, image[i].elemSize1() * input.size().width * input.size().height);
  }*/
}

void ScaleSpaceImage::show(std::string prefix, std::string processor, unsigned int image_number, bool debug)
{
  static int image_nr = 0;
  cv::Mat to_write;

  std::string image_number_string;
  if (image_number < 10)
  {
    image_number_string = "0";
  }
  image_number_string += std::to_string(image_number);
  cv::imwrite(prefix + "_" + processor + image_number_string + "_original.bmp", input);

  for (unsigned int i = 0; i < nr_scales; ++i)
  {
    std::string s;
    s = prefix + "_" + processor + "_";
    s += image_number_string;
    if (i < 10)
    {
      s += "0";
    }
    s += "_";
    s += std::to_string(i);
    if (debug)
    {
      std::string ds1 = s + "_1.bmp";
      std::string ds1txt = s + "_1.txt";
      std::ofstream im1(ds1txt);

      scale_space_images[0][i].convertTo(to_write, CV_8UC1, 255.0);
      cv::imwrite(ds1.c_str(), to_write);
      im1 << scale_space_images[0][i];

      if (scale_space_images.size() > 1)
      {
        std::string ds2 = s + "_2.bmp";
        std::string ds2txt = s + "_2.txt";
        std::ofstream im2(ds2txt);

        scale_space_images[1][i].convertTo(to_write, CV_8UC1, 255.0);
        cv::imwrite(ds2.c_str(), to_write);
        im2 << scale_space_images[1][i];
      }
    }
    s += ".bmp";

    if (gaussian)
    {
      scale_space_images[0][i].convertTo(to_write, CV_8UC1, 255.0);
    }
    else
    {
      to_write = output[i];
    }
    cv::imwrite(s.c_str(), to_write);
  }
  image_nr++;
}

void ScaleSpaceImage::show(cv::Mat & blobs, std::vector<float> & sigmas)
{
  /*cv::Mat tmp2(input.size().height, input.size().width, image[0].type()), tmp;
  memcpy(tmp2.data, getDataForScale(0), image[0].elemSize() * input.size().width * input.size().height);
  tmp2.convertTo(tmp, CV_8UC1, 256.0);*/
  cv::Mat tmp(input.clone());
  for (int i=0; i < input.size().height; ++i)
  {
    for (int j=0; j < input.size().width; ++j)
    {
      if (uchar scale = blobs.at<uchar>(i,j))
      {
        cv::circle(tmp, cv::Point2i(j, i), static_cast<int>(sigmas[scale - 1] * 1.412f + 0.5f), cv::Scalar(127), 1);
      }
    }
  }
  cv::imwrite("result.bmp",tmp);
}

unsigned int ScaleSpaceImage::getOneImageSize() const
{
  unsigned int size = input.size().height * input.size().width;
  if (temporary_image_type == CV_32FC1)
  {
    return size * sizeof(float);
  }
  else if (temporary_image_type == CV_8UC1)
  {
    return size * sizeof(unsigned char);
  }
  throw ScaleSpaceImageException("Not supported output image type");
}

void * ScaleSpaceImage::getDataForOutput(unsigned int scale)
{
  checkScale(scale);
  return output[scale].data;
}

cv::Mat & ScaleSpaceImage::getOutput(unsigned int scale)
{
  checkScale(scale);
  return output[scale];
}

void ScaleSpaceImage::checkScale(unsigned int scale)
{
  if (scale >= nr_scales)
  {
    throw ScaleSpaceImageException("Wrong scale parameter: " + std::to_string(scale) + ". Can be 0 -" + std::to_string(nr_scales - 1));
  }
}

cv::Size ScaleSpaceImage::size() const
{
  return input.size();
}

void * ScaleSpaceImage::getInputData() const
{
  return input.data;
}

int ScaleSpaceImage::type() const
{
  return input.type();
}

cv::Mat & ScaleSpaceImage::getInput()
{
  return input;
}

cv::Mat & ScaleSpaceImage::getImageForScale(unsigned int scale, unsigned int image_number)
{
  if (image_number >= nr_images)
  {
    throw ScaleSpaceImageException("Wrong image number parameter: " + std::to_string(image_number) + ". Can be 0 -" + std::to_string(nr_images - 1));  
  }
  checkScale(scale);
  if (!scale_space_images[image_number][scale].isContinuous())
  {
    throw ScaleSpaceImageException("Data is not continuous");
  }
  return scale_space_images[image_number][scale];
}

void ScaleSpaceImage::setGaussian(bool g)
{
  gaussian = g;
}
