#include "ScaleSpaceOpenCV.h"

#include <opencv2\imgproc\imgproc.hpp>

//#define SS_DEBUG
#define SS_INFO

#ifdef SS_DEBUG
#include <iostream>
#include <fstream>
#endif
#include <fstream>

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

#ifdef SS_INFO
  image.show("gaussian");
#endif

  (this->*doMode)(image);

#ifdef SS_INFO
  image.show("mode");
#endif

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
  input.convertTo(output, temp_image_type, 1.0f/255.0f);

#ifdef SS_DEBUG
  std::ofstream of("image.txt");
  of << output;
#endif

}

void ScaleSpaceOpenCV::doBlob(ScaleSpaceImage& image) const
{
  cv::Mat Lxx, Lxy, Lyy;

  for (unsigned int i = 0; i < nr_scales; ++i)
  {
    cv::Mat L;

    calcSecondDeriteratives(image.getImageForScale(i), Lxx, Lxy, Lyy);

    std::ofstream of("pochodnaLxx.txt");
    of << Lxx;
  
    L = abs(Lxx + Lyy);

    image.getImageForScale(i) = L;
  }
}

void ScaleSpaceOpenCV::doCorner(ScaleSpaceImage& image) const
{
  
}

void ScaleSpaceOpenCV::doEdge(ScaleSpaceImage& image) const
{
  
}

void ScaleSpaceOpenCV::doRidge(ScaleSpaceImage& image) const
{
  cv::Mat Lx, Ly, Lxx, Lxy, Lyy, L1, L2;

  for (unsigned int i = 0; i < nr_scales; ++i)
  {
    calcFirstDeriteratives(image.getImageForScale(i), Lx, Ly);
    calcSecondDeriteratives(image.getImageForScale(i), Lxx, Lxy, Lyy);
  
    L1 = Lx * Ly * (Lxx - Lyy) - (Lx * Lx - Ly * Ly) * Lxy;
    //TODO: clear for zeros
    //tODO: continue
    L2 = L1;
  }
  
}

void ScaleSpaceOpenCV::doPure(ScaleSpaceImage& image) const
{
  
}

void ScaleSpaceOpenCV::calcDX(cv::Mat& in, cv::Mat& out) const
{
  cv::Mat kernel(3,3, CV_32FC1, cv::Scalar(0.0));
  kernel.at<float>(0, 0) = -0.5;
  kernel.at<float>(0, 2) = 0.5;
  kernel.at<float>(1, 0) = -1;
  kernel.at<float>(1, 2) = 1;
  kernel.at<float>(2, 0) = -0.5;
  kernel.at<float>(2, 2) = 0.5;

  cv::filter2D(in, out, -1, kernel);
}

void ScaleSpaceOpenCV::calcDY(cv::Mat& in, cv::Mat& out) const
{
  cv::Mat kernel(3,3, CV_32FC1, cv::Scalar(0.0));
  kernel.at<float>(0, 0) = -0.5;
  kernel.at<float>(2, 0) = 0.5;
  kernel.at<float>(0, 1) = -1;
  kernel.at<float>(2, 1) = 1;
  kernel.at<float>(0, 2) = -0.5;
  kernel.at<float>(2, 2) = 0.5;

  cv::filter2D(in, out, -1, kernel);
}

void ScaleSpaceOpenCV::calcDXX(cv::Mat& in, cv::Mat& out) const
{
  cv::Mat kernel(3,3, CV_32FC1, cv::Scalar(0.0));
  kernel.at<float>(0, 0) = 1.0f/12.0f;
  kernel.at<float>(0, 1) = -1.0f/6.0f;
  kernel.at<float>(0, 2) = 1.0f/12.0f;
  kernel.at<float>(1, 0) = 5.0f/6.0f;
  kernel.at<float>(1, 1) = -5.0f/3.0f;
  kernel.at<float>(1, 2) = 5.0f/6.0f;
  kernel.at<float>(2, 0) = 1.0f/12.0f;
  kernel.at<float>(2, 1) = -1.0f/6.0f;
  kernel.at<float>(2, 2) = 1.0f/12.0f;

  cv::filter2D(in, out, -1, kernel);

  { 
    std::ofstream of("kernel.txt");
    of << kernel;
}

  { 
    std::ofstream of("pochodna in.txt");
    of << in;
}
  std::ofstream of("pochodnaLxx out.txt");
  of << out;
}

void ScaleSpaceOpenCV::calcDYY(cv::Mat& in, cv::Mat& out) const
{
  cv::Mat kernel(3,3, CV_32FC1, cv::Scalar(0.0));
  kernel.at<float>(0, 0) = 1.0f/12.0f;
  kernel.at<float>(0, 1) = 5.0f/6.0f;
  kernel.at<float>(0, 2) = 1.0f/12.0f;
  kernel.at<float>(1, 0) = -1.0f/6.0f;
  kernel.at<float>(1, 1) = -5.0f/3.0f;
  kernel.at<float>(1, 2) = -1.0f/6.0f;
  kernel.at<float>(2, 0) = 1.0f/12.0f;
  kernel.at<float>(2, 1) = 5.0f/6.0f;
  kernel.at<float>(2, 2) = 1.0f/12.0f;

  cv::filter2D(in, out, -1, kernel);
}

void ScaleSpaceOpenCV::calcDXY(cv::Mat& in, cv::Mat& out) const
{
  cv::Mat kernel(3,3, CV_32FC1, cv::Scalar(0.0));
  kernel.at<float>(0, 0) = 1.0f/4.0f;
  kernel.at<float>(0, 2) = -1.0f/4.0f;
  kernel.at<float>(2, 0) = -1.0f/4.0f;
  kernel.at<float>(2, 2) = 1.0f/4.0f;

  cv::filter2D(in, out, -1, kernel);
}

void ScaleSpaceOpenCV::calcDXXX(cv::Mat& in, cv::Mat& out) const
{
  cv::Mat kernel(3,3, CV_32FC1, cv::Scalar(0.0));
  kernel.at<float>(0, 0) = 1.0f/2.0f;
  kernel.at<float>(0, 1) = 1.0;
  kernel.at<float>(0, 2) = 1.0f/2.0f;
  kernel.at<float>(2, 0) = -1.0f/2.0f;
  kernel.at<float>(2, 1) = -1.0;
  kernel.at<float>(2, 2) = -1.0f/2.0f;

  cv::filter2D(in, out, -1, kernel);
}

void ScaleSpaceOpenCV::calcDXXY(cv::Mat& in, cv::Mat& out) const
{
  cv::Mat kernel(3,3, CV_32FC1, cv::Scalar(0.0));
  kernel.at<float>(0, 0) = -1.0f/8.0f;
  kernel.at<float>(1, 0) = 1.0f/4.0f;
  kernel.at<float>(2, 0) = -1.0f/8.0f;
  kernel.at<float>(0, 2) = 1.0f/8.0f;
  kernel.at<float>(1, 2) = -1.0f/4.0f;
  kernel.at<float>(2, 2) = 1.0f/8.0f;

  cv::filter2D(in, out, -1, kernel);
}

void ScaleSpaceOpenCV::calcDXYY(cv::Mat& in, cv::Mat& out) const
{
  cv::Mat kernel(3,3, CV_32FC1, cv::Scalar(0.0));
  kernel.at<float>(0, 0) = -1.0f/8.0f;
  kernel.at<float>(0, 1) = 1.0f/4.0f;
  kernel.at<float>(0, 2) = -1.0f/8.0f;
  kernel.at<float>(2, 0) = 1.0f/8.0f;
  kernel.at<float>(2, 1) = -1.0f/4.0f;
  kernel.at<float>(2, 2) = 1.0f/8.0f;

  cv::filter2D(in, out, -1, kernel);
}

void ScaleSpaceOpenCV::calcDYYY(cv::Mat& in, cv::Mat& out) const
{
  cv::Mat kernel(3,3, CV_32FC1, cv::Scalar(0.0));
  kernel.at<float>(0, 0) = 1.0f/2.0f;
  kernel.at<float>(1, 0) = 1.0;
  kernel.at<float>(2, 0) = 1.0f/2.0f;
  kernel.at<float>(0, 2) = -1.0f/2.0f;
  kernel.at<float>(1, 2) = -1.0;
  kernel.at<float>(2, 2) = -1.0f/2.0f;

  cv::filter2D(in, out, -1, kernel);
}

void ScaleSpaceOpenCV::calcFirstDeriteratives(cv::Mat& in, cv::Mat& Lx, cv::Mat& Ly) const
{
  calcDX(in, Lx);
  calcDY(in, Ly);
}

void ScaleSpaceOpenCV::calcSecondDeriteratives(cv::Mat& in, cv::Mat& Lxx, cv::Mat& Lxy, cv::Mat& Lyy) const
{
  calcDXX(in, Lxx);
  calcDXY(in, Lxy);
  calcDYY(in, Lyy);

  
  std::ofstream of("pochodnaLxx second.txt");
  of << Lxx;
}

void ScaleSpaceOpenCV::calcThirdDeriteratives(cv::Mat& in, cv::Mat& Lxxx, cv::Mat& Lxxy, cv::Mat& Lxyy, cv::Mat& Lyyy) const
{
  calcDXXX(in, Lxxx);
  calcDXXY(in, Lxxy);
  calcDXYY(in, Lxxy);
  calcDYYY(in, Lyyy);
}
