#include "ScaleSpaceImage.h"

#include <opencv\highgui.h>

#include <fstream>

//#define DEBUG_SS
#ifdef DEBUG_SS
//#define DEBUG_SS_TWO_OUTPUT
#endif

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

void ScaleSpaceImage::show(std::string fn)
{
  static int image_nr = 0;
  cv::Mat tmp(input.size().height, input.size().width, scale_space_images[0][0].type());
  memcpy(tmp.data, getDataForScale(0), scale_space_images[0][0].elemSize() * input.size().width * input.size().height);
  cv::Mat tmp2;
  if (temporary_image_type == CV_32FC1)
  {
    tmp.convertTo(tmp2, CV_8UC1, 255.0);
  }
  else if (temporary_image_type == CV_8UC1)
  {
    tmp2 = tmp;
  }
  else
  {
    throw ScaleSpaceImageException("Not supported output image type");
  }
  cv::imwrite("original.bmp", input);
  //cv::imwrite("original.bmp", tmp2);

  for (unsigned int i = 0; i < nr_scales; ++i)
  {
    std::string s;
    s = fn + std::to_string(i) + ".bmp";
    memcpy(tmp.data, getDataForScale(i), scale_space_images[0][0].elemSize() * input.size().width * input.size().height);
#ifdef DEBUG_SS
    std::ofstream out("data1" + std::to_string(image_nr) +  std::to_string(i) + ".txt");
    out << tmp; //very slow
    out.close();
    //for some algorithms
#ifdef DEBUG_SS_TWO_OUTPUT
    cv::Mat tmp3(input.size().height, input.size().width, scale_space_images[1].type());
    memcpy(tmp3.data, getDataForScale(i, 1), scale_space_images[1][0].elemSize() * input.size().width * input.size().height);
    std::ofstream out2("data2" + std::to_string(image_nr) +  std::to_string(i) + ".txt");
    out2 << tmp3;
    out2.close();//*/ //very slow
#endif //DEBUG_SS_TWO_OUTPUT
#endif //DEBUG_SS
    cv::Mat tmp2;
    if (temporary_image_type == CV_32FC1)
    {
      tmp.convertTo(tmp2, CV_8UC1, 255.0);
    }
    else if (temporary_image_type == CV_8UC1)
    {
      tmp2 = tmp;
    }
    else
    {
      throw ScaleSpaceImageException("Not supported output image type");
    }
//    cv::imwrite(s.c_str(), tmp2);
    cv::imwrite(s.c_str(), output[i]);
    cv::imwrite((s+"_tmp.bmp").c_str(), tmp2);
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
