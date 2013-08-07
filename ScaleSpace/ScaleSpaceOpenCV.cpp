#include "ScaleSpaceOpenCV.h"

#include <opencv2\imgproc\imgproc.hpp>

ScaleSpaceOpenCV::ScaleSpaceOpenCV(ScaleSpaceMode mode)
{
  changeToFloat = nullptr;
  temp_image_type = CV_32FC1;
}


ScaleSpaceOpenCV::~ScaleSpaceOpenCV(void)
{
}

void ScaleSpaceOpenCV::prepare(ScaleSpaceSourceImageType si_type, ScaleSpaceOutputType out_type)
{
  switch (si_type)
  {
  case ScaleSpaceSourceImageType::Gray:
    changeToFloat = &ScaleSpaceOpenCV::changeBayerToFloat;
    break;
  case ScaleSpaceSourceImageType::Bayer:
    changeToFloat = &ScaleSpaceOpenCV::changeGrayToFloat;
    break;
  }
}

void ScaleSpaceOpenCV::processImage(ScaleSpaceImage & image)
{
  //create output
  image.createImage(nr_scales, temp_image_type);

  doGaussian(image);
}

void ScaleSpaceOpenCV::doGaussian(ScaleSpaceImage & image)
{
  cv::Mat input;

  (this->*changeToFloat)(image.getInput(), input);
  
  for (unsigned int i = 0; i < nr_scales; ++i)
  {
    cv::Mat gaussian = getGaussianForScale(i);

    cv::filter2D(input, image.getImageForScale(i), -1, gaussian);

    image.getImageForScale(i) *= sigmas[i];
  }

    
}

void ScaleSpaceOpenCV::changeBayerToFloat(cv::Mat& input, cv::Mat& output) const
{
  
}

void ScaleSpaceOpenCV::changeGrayToFloat(cv::Mat& input, cv::Mat& output) const
{
  
}
  