#include "ScaleSpaceOpenCV.h"

#include <opencv2\imgproc\imgproc.hpp>

//#define SS_DEBUG

#ifdef SS_DEBUG
#include <iostream>
#include <fstream>
#endif

ScaleSpaceOpenCV::ScaleSpaceOpenCV(ScaleSpaceMode mode)
{
  changeToFloat = nullptr;
  temp_image_type = CV_32FC1;

  switch (mode)
  {
  case ScaleSpaceMode::Blobs:
    doMode = &ScaleSpaceOpenCV::doBlob;
    break;
  case ScaleSpaceMode::Corners:
    doMode = &ScaleSpaceOpenCV::doCorner;
    break;
  case ScaleSpaceMode::Edges:
    doMode = &ScaleSpaceOpenCV::doEdge;
    break;
  case ScaleSpaceMode::Ridges:
    doMode = &ScaleSpaceOpenCV::doRidge;
    break;
  case ScaleSpaceMode::Pure:
    doMode = &ScaleSpaceOpenCV::doPure;
    break;
  default:
    doMode = nullptr;
    break;
  }
}


ScaleSpaceOpenCV::~ScaleSpaceOpenCV(void)
{
}

void ScaleSpaceOpenCV::prepare(ScaleSpaceSourceImageType si_type, ScaleSpaceOutputType out_type)
{
  switch (si_type)
  {
  case ScaleSpaceSourceImageType::Gray:
    changeToFloat = &ScaleSpaceOpenCV::changeGrayToFloat;
    break;
  case ScaleSpaceSourceImageType::Bayer:
    changeToFloat = &ScaleSpaceOpenCV::changeBayerToFloat;
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

  if (!doMode)
  {
    throw ScaleSpaceException("Mode is not set");
  }

  (this->*changeToFloat)(image.getInput(), input);
  
  for (unsigned int i = 0; i < nr_scales; ++i)
  {
    cv::Mat gaussian = getGaussianForScale(i);

#ifdef SS_DEBUG
    std::cout << gaussian <<"\n";
#endif

    cv::filter2D(input, image.getImageForScale(i), -1, gaussian);
    
#ifdef SS_DEBUG
    std::ofstream of("pogau.txt");
    of << image.getImageForScale(i);
#endif
  }

    
}

void ScaleSpaceOpenCV::changeBayerToFloat(cv::Mat& input, cv::Mat& output) const
{
  //TODO: dopisa� i sprawdzi� mode
}

void ScaleSpaceOpenCV::changeGrayToFloat(cv::Mat& input, cv::Mat& output) const
{
  //cv::cvtColor(input, output, CV_RGB2GRAY);
//  input.copyTo(output);
  input.convertTo(output, temp_image_type, 1.0/255.0);

#ifdef SS_DEBUG
  std::ofstream of("image.txt");
  of << output;
#endif

}
  
void ScaleSpaceOpenCV::doBlob(cv::Mat&, ScaleSpaceImage& image) const
{
  
}

void ScaleSpaceOpenCV::doCorner(cv::Mat&, ScaleSpaceImage& image) const
{
  
}

void ScaleSpaceOpenCV::doEdge(cv::Mat&, ScaleSpaceImage& image) const
{
  
}

void ScaleSpaceOpenCV::doRidge(cv::Mat&, ScaleSpaceImage& image) const
{
  
}

void ScaleSpaceOpenCV::doPure(cv::Mat&, ScaleSpaceImage& image) const
{
  
}
