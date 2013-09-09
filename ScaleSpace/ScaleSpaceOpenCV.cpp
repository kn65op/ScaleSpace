#include "ScaleSpaceOpenCV.h"
#include <Stoper.h>

#include <opencv2\imgproc\imgproc.hpp>

//#define SS_DEBUG
#define SS_INFO

using namespace TTime;

ScaleSpaceOpenCV::ScaleSpaceOpenCV(ScaleSpaceMode mode) :
  ScaleSpace(mode)
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

void ScaleSpaceOpenCV::process(ScaleSpaceImage & image, bool first_image)
{
  run_stoper = !first_image || parameters.calc_first_image;

  //create output
  image.createImage(nr_scales, temp_image_type, nr_images);

  if (run_stoper)
  {
    Stoper::start("gaussian", false);
  }
  doGaussian(image);
  if (run_stoper)
  {
    Stoper::stop("gaussian");
  }

  (this->*doMode)(image);

  if (doPostProcessing)
  {
    (this->*doPostProcessing)(image);
  }
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

    if (run_stoper)
    {
      Stoper::start("gaussian" + std::to_string(i) ,false);
    }
    cv::filter2D(input, image.getImageForScale(i), -1, gaussian);
    if (run_stoper)
    {
      Stoper::stop("gaussian" + std::to_string(i));
    }
  }

    
}

void ScaleSpaceOpenCV::changeBayerToFloat(cv::Mat& input, cv::Mat& output) const
{
  convertInputFromBayer(input, output);
}

void ScaleSpaceOpenCV::changeGrayToFloat(cv::Mat& input, cv::Mat& output) const
{
  convertInput(input, output);
}

void ScaleSpaceOpenCV::doBlob(ScaleSpaceImage& image) const
{
  cv::Mat Lxx, Lxy, Lyy;

  for (unsigned int i = 0; i < nr_scales; ++i)
  {
    cv::Mat L;

    calcSecondDeriteratives(image.getImageForScale(i), Lxx, Lxy, Lyy);
  
    calcBlob(Lxx, Lyy, sigmas[i], L);

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
  
  filter2D(in, out, kernel);
}

void ScaleSpaceOpenCV::calcDXY(cv::Mat& in, cv::Mat& out) const
{
  cv::Mat kernel(3,3, CV_32FC1, cv::Scalar(0.0));
  kernel.at<float>(0, 0) = 1.0f/4.0f;
  kernel.at<float>(0, 2) = -1.0f/4.0f;
  kernel.at<float>(2, 0) = -1.0f/4.0f;
  kernel.at<float>(2, 2) = 1.0f/4.0f;
  
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
        if (!i && !j) continue;
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
