#include "ScaleSpaceOpenCV.h"



ScaleSpaceOpenCV::ScaleSpaceOpenCV(ScaleSpaceMode mode)
{
  changeToFloat = nullptr;
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
  }

    
}

void ScaleSpaceOpenCV::changeBayerToFloat(cv::Mat& input, cv::Mat& output) const
{
  
}

void ScaleSpaceOpenCV::changeGrayToFloat(cv::Mat& input, cv::Mat& output) const
{
  
}
  