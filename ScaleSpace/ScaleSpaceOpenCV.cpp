#include "ScaleSpaceOpenCV.h"
#include <Stoper.h>

#include <opencv2\imgproc\imgproc.hpp>

//#define SS_DEBUG
#define SS_INFO

#ifdef SS_INFO
#include <iostream>
#endif

#ifdef SS_DEBUG
#include <fstream>
#endif
#include <fstream> //tmp

using namespace TTime;

ScaleSpaceOpenCV::ScaleSpaceOpenCV(ScaleSpaceMode mode)
{
  changeToFloat = nullptr;
  temp_image_type = CV_32FC1;
  nr_images = 1;

  switch (mode)
  {
  case ScaleSpaceMode::Blobs:
    doMode = &ScaleSpaceOpenCV::doBlob;
    doPostProcessing = &ScaleSpaceOpenCV::findMaxInScale;
    break;
  case ScaleSpaceMode::Corners:
    doMode = &ScaleSpaceOpenCV::doCorner;
    doPostProcessing = &ScaleSpaceOpenCV::findMaxInScale;
    break;
  case ScaleSpaceMode::Edges:
    doMode = &ScaleSpaceOpenCV::doEdge;
    doPostProcessing = &ScaleSpaceOpenCV::findEdgeMax;
    nr_images = 2;
    break;
  case ScaleSpaceMode::Ridges:
    doMode = &ScaleSpaceOpenCV::doRidge;
    doPostProcessing = &ScaleSpaceOpenCV::findRidgeMax;
    nr_images = 2;
    break;
  case ScaleSpaceMode::Pure:
    doMode = &ScaleSpaceOpenCV::doPure;
    doPostProcessing = nullptr;
    break;
  default:
    doMode = nullptr;
    doPostProcessing = nullptr;
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
  image.createImage(nr_scales, temp_image_type, nr_images);

  Stoper::start("gaussian", false);
  doGaussian(image);
  Stoper::stop("gaussian");

#ifdef SS_INFO
  std::cout << "gaussian\n";
#endif

  (this->*doMode)(image);

#ifdef SS_INFO
  std::cout << "mode\n";
#endif

  if (doPostProcessing)
  {
    (this->*doPostProcessing)(image);
  }
#ifdef SS_INFO
  std::cout << "post ptocessing\n";
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
  
    calcBlob(Lxx, Lyy, sigmas[i], L);

#ifdef SS_DEBUG
    std::ofstream of("pochodnaL.txt");
    of << L;
#endif

    image.getImageForScale(i) = L;
  }
}

void ScaleSpaceOpenCV::doCorner(ScaleSpaceImage& image) const
{
  cv::Mat Lx, Ly, Lxx, Lxy, Lyy;

    for (unsigned int i = 0; i < nr_scales; ++i)
  {
    cv::Mat k;

    calcFirstDeriteratives(image.getImageForScale(i), Lx, Ly);
    calcSecondDeriteratives(image.getImageForScale(i), Lxx, Lxy, Lyy);

    calcCorner(Lx, Ly, Lxx, Lxy, Lyy, k);
    
#ifdef SS_DEBUG
    std::ofstream of("k.txt");
    of << k;
#endif
    image.getImageForScale(i) = k;
  }
}

void ScaleSpaceOpenCV::doEdge(ScaleSpaceImage& image) const
{
  cv::Mat Lx, Ly, Lxx, Lxy, Lyy, L1, L2;
  cv::Mat Lxxx, Lxxy, Lxyy, Lyyy;

  for (unsigned int i = 0; i < nr_scales; ++i)
  {
    calcFirstDeriteratives(image.getImageForScale(i), Lx, Ly);
    calcSecondDeriteratives(image.getImageForScale(i), Lxx, Lxy, Lyy);
    calcThirdDeriteratives(image.getImageForScale(i), Lxxx, Lxxy, Lxyy, Lyyy);

    calcEdge(Lx, Ly, Lxx, Lxy, Lyy, Lxxx, Lxxy, Lxyy, Lyyy, L1, L2);
#define SS_DEBUG
#ifdef SS_DEBUG
    std::ofstream of("L1.txt");
    of << L1;
#endif
#ifdef SS_DEBUG
    std::ofstream of2("L2.txt");
    of2 << L2;
#endif
#undef SS_DEBUG

    image.getImageForScale(i, 0) = L1;
    image.getImageForScale(i, 1) = L2;
  }
}

void ScaleSpaceOpenCV::doRidge(ScaleSpaceImage& image) const
{
  cv::Mat Lx, Ly, Lxx, Lxy, Lyy, L1, L2;

  for (unsigned int i = 0; i < nr_scales; ++i)
  {
    calcFirstDeriteratives(image.getImageForScale(i), Lx, Ly);
    calcSecondDeriteratives(image.getImageForScale(i), Lxx, Lxy, Lyy);

    calcRidge(Lx, Ly, Lxx, Lxy, Lyy, L1, L2);

#ifdef SS_DEBUG
    std::ofstream of("L1.txt");
    of << L1;
#endif

#ifdef SS_DEBUG
    std::ofstream of2("L2.txt");
    of2 << L2;
#endif

    image.getImageForScale(i, 0) = L1;
    image.getImageForScale(i, 1) = L2;
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
  
    { 
    std::ofstream of("kernelX.txt");
    of << kernel;
}

  filter2D(in, out, kernel);
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
  
    { 
    std::ofstream of("kernelY.txt");
    of << kernel;
}
  filter2D(in, out, kernel);
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

  filter2D(in, out, kernel);
  { 
    std::ofstream of("kernelXX.txt");
    of << kernel;
}
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
  
    { 
    std::ofstream of("kernelYY.txt");
    of << kernel;
}
  filter2D(in, out, kernel);
}

void ScaleSpaceOpenCV::calcDXY(cv::Mat& in, cv::Mat& out) const
{
  cv::Mat kernel(3,3, CV_32FC1, cv::Scalar(0.0));
  kernel.at<float>(0, 0) = 1.0f/4.0f;
  kernel.at<float>(0, 2) = -1.0f/4.0f;
  kernel.at<float>(2, 0) = -1.0f/4.0f;
  kernel.at<float>(2, 2) = 1.0f/4.0f;
  
    { 
    std::ofstream of("kernelXY.txt");
    of << kernel;
}
  filter2D(in, out, kernel);
}

void ScaleSpaceOpenCV::calcDYYY(cv::Mat& in, cv::Mat& out) const
{
  cv::Mat kernel(3,3, CV_32FC1, cv::Scalar(0.0));
  kernel.at<float>(0, 0) = 1.0f/2.0f;
  kernel.at<float>(0, 1) = 1.0;
  kernel.at<float>(0, 2) = 1.0f/2.0f;
  kernel.at<float>(2, 0) = -1.0f/2.0f;
  kernel.at<float>(2, 1) = -1.0;
  kernel.at<float>(2, 2) = -1.0f/2.0f;
  
    { 
    std::ofstream of("kernelYYY.txt");
    of << kernel;
}

  filter2D(in, out, kernel);
}

void ScaleSpaceOpenCV::calcDXYY(cv::Mat& in, cv::Mat& out) const
{
  cv::Mat kernel(3,3, CV_32FC1, cv::Scalar(0.0));
  kernel.at<float>(0, 0) = -1.0f/8.0f;
  kernel.at<float>(1, 0) = 1.0f/4.0f;
  kernel.at<float>(2, 0) = -1.0f/8.0f;
  kernel.at<float>(0, 2) = 1.0f/8.0f;
  kernel.at<float>(1, 2) = -1.0f/4.0f;
  kernel.at<float>(2, 2) = 1.0f/8.0f;
  { 
    std::ofstream of("kernelXYY.txt");
    of << kernel;
}
  filter2D(in, out, kernel);
}

void ScaleSpaceOpenCV::calcDXXY(cv::Mat& in, cv::Mat& out) const
{
  cv::Mat kernel(3,3, CV_32FC1, cv::Scalar(0.0));
  kernel.at<float>(0, 0) = -1.0f/8.0f;
  kernel.at<float>(0, 1) = 1.0f/4.0f;
  kernel.at<float>(0, 2) = -1.0f/8.0f;
  kernel.at<float>(2, 0) = 1.0f/8.0f;
  kernel.at<float>(2, 1) = -1.0f/4.0f;
  kernel.at<float>(2, 2) = 1.0f/8.0f;
    { 
    std::ofstream of("kernelXXY.txt");
    of << kernel;
}
  filter2D(in, out, kernel);
}

void ScaleSpaceOpenCV::calcDXXX(cv::Mat& in, cv::Mat& out) const
{
  cv::Mat kernel(3,3, CV_32FC1, cv::Scalar(0.0));
  kernel.at<float>(0, 0) = 1.0f/2.0f;
  kernel.at<float>(1, 0) = 1.0;
  kernel.at<float>(2, 0) = 1.0f/2.0f;
  kernel.at<float>(0, 2) = -1.0f/2.0f;
  kernel.at<float>(1, 2) = -1.0;
  kernel.at<float>(2, 2) = -1.0f/2.0f;
  
    { 
    std::ofstream of("kernelXXX.txt");
    of << kernel;
}
  filter2D(in, out, kernel);
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
#ifdef SS_DEBUG
  
  std::ofstream of("pochodnaLxx second.txt");
  of << Lxx;
#endif
}

void ScaleSpaceOpenCV::calcThirdDeriteratives(cv::Mat& in, cv::Mat& Lxxx, cv::Mat& Lxxy, cv::Mat& Lxyy, cv::Mat& Lyyy) const
{
  calcDXXX(in, Lxxx);
  calcDXXY(in, Lxxy);
  calcDXYY(in, Lxyy);
  calcDYYY(in, Lyyy);
}

void ScaleSpaceOpenCV::findEdgeMax(ScaleSpaceImage& image) const
{
  for (unsigned int i=0; i < nr_scales; ++i)
  {
    calcEdgeMax(image.getImageForScale(i, 0), image.getImageForScale(i, 1), image.getOutput(i));
  }
}

void ScaleSpaceOpenCV::findMaxInScale(ScaleSpaceImage& image) const
{
  for (unsigned int i=0; i < nr_scales; ++i)
  {
    calcMaxInScale(image.getImageForScale(i), image.getOutput(i));
  }
}

void ScaleSpaceOpenCV::findRidgeMax(ScaleSpaceImage& image) const
{
  for (unsigned int i=0; i < nr_scales; ++i)
  {
    calcRidgeMax(image.getImageForScale(i, 0), image.getImageForScale(i, 1), image.getOutput(i));
  }
}


void ScaleSpaceOpenCV::calcRidgeMax(cv::Mat& L1, cv::Mat& L2, cv::Mat& out) const
{
  processTwoImagesNonBorder(L1 , L2, out, [] (cv::Mat & in, cv::Mat & in_sec, int x, int y)->unsigned char
  {
    float centre = in.at<float>(x,y);
    if (centre == 0 && in_sec.at<float>(x, y) > 0)
    {
      return 255;
    }
    return 0;
  });
}

void ScaleSpaceOpenCV::calcEdgeMax(cv::Mat& L1, cv::Mat& L2, cv::Mat& out) const
{
  processTwoImagesNonBorder(L1, L2, out, [] (cv::Mat & in, cv::Mat & in_sec, int x, int y)->unsigned char
  {
    float centre = in.at<float>(x,y);
    for (int i = -1; i < 2; ++i)
    {
      for (int j = -1; j < 2; ++j)
      {
        if (i && j) continue;
        if (in.at<float>(x + i, y + j) * centre < 0 && in_sec.at<float>(x, y) < 0)
        {
          return 255;
        }
      }
    }
    return 0;
  });
}

void ScaleSpaceOpenCV::calcMaxInScale(cv::Mat& L, cv::Mat& out) const
{
  processImageNonBorder(L, out, [] (cv::Mat & in, int x, int y)->unsigned char
  {
    float centre = in.at<float>(x,y);
    for (int i = -1; i < 2; ++i)
    {
      for (int j = -1; j < 2; ++j)
      {
        if (centre < 1e-5)
        {
          return 0;
        }
        if (in.at<float>(x + i, y + j) >= centre && i && j)
        {
          return 0;
        }
      }
    }
    return 255;
  });
}


void ScaleSpaceOpenCV::processImage(cv::Mat & in, cv::Mat & out, std::function<float (float)> fun) const
{
  cv::Size size = in.size();
 
  for (int i=0; i<size.height; ++i)
  {
    for (int j=0; j<size.width; ++j)
    {
      out.at<float>(i,j) = fun(in.at<float>(i,j));
    }
  }
}

void ScaleSpaceOpenCV::processImageNonBorder(cv::Mat & in, cv::Mat & out, std::function<unsigned char (cv::Mat&,int,int)> fun) const
{
  //process non border pixels
  cv::Size size = in.size();
  --size.width;
  --size.height;

  for (int i=1; i<size.height; ++i)
  {
    for (int j=1; j<size.width; ++j)
    {
      out.at<unsigned char>(i,j) = fun(in, i, j);
    }
  }
}

void ScaleSpaceOpenCV::processTwoImagesNonBorder(cv::Mat & in, cv::Mat & in_sec, cv::Mat & out, std::function<unsigned char (cv::Mat &, cv::Mat &, int, int)> fun) const
{
  //process non border pixels
  cv::Size size = in.size();
  --size.width;
  --size.height;

  for (int i=1; i<size.height; ++i)
  {
    for (int j=1; j<size.width; ++j)
    {
      out.at<unsigned char>(i,j) = fun(in, in_sec, i, j);
    }
  }
}

void ScaleSpaceOpenCV::setLowValuesToZero(cv::Mat& mat) const
{
  processImage(mat, mat, [] (float in)->float
  {
    if (fabs(in) < 1e-5)
    {
      return 0;
    }
    return in;
  });
}
