#include "ScaleSpaceOpenCVCPU.h"
#include "ScaleSpaceOpenCVGPU.h"

#include <opencv2\imgproc\imgproc.hpp>

ScaleSpaceOpenCVCPU::ScaleSpaceOpenCVCPU(ScaleSpaceMode mode)
  : ScaleSpaceOpenCV(mode)
{
}


ScaleSpaceOpenCVCPU::~ScaleSpaceOpenCVCPU(void)
{
}

void ScaleSpaceOpenCVCPU::filter2D(cv::Mat& src, cv::Mat& dst, cv::Mat& kernel) const
{
  cv::filter2D(src, dst, -1, kernel);
}

void ScaleSpaceOpenCVCPU::calcBlob(cv::Mat& Lxx, cv::Mat& Lyy, float sigma, cv::Mat& L) const
{
  L = abs(Lxx + Lyy) * sigma;
}

void ScaleSpaceOpenCVCPU::calcCorner(cv::Mat& Lx, cv::Mat& Ly, cv::Mat& Lxx, cv::Mat& Lxy, cv::Mat& Lyy, cv::Mat& k) const
{
  k = abs(Lx.mul(Lx).mul(Lyy) +  Ly.mul(Ly).mul(Lxx) - Lx.mul(Ly.mul(Lxy)) * 2.0);
}

void ScaleSpaceOpenCVCPU::calcEdge(cv::Mat& Lx, cv::Mat& Ly, cv::Mat& Lxx, cv::Mat& Lxy, cv::Mat& Lyy, cv::Mat& Lxxx, cv::Mat& Lxxy, cv::Mat& Lxyy, cv::Mat& Lyyy, cv::Mat& L1, cv::Mat& L2) const
{
  L1 = Lx.mul(Lx.mul(Lxx)) + 2 * Lx.mul(Ly.mul(Lxy)) + Ly.mul(Ly.mul(Lyy));
  setLowValuesToZero(L1);

  L2 = Lx.mul(Lx).mul(Lx).mul(Lxxx) + 3 * Lx.mul(Lx).mul(Ly).mul(Lxxy) + 3 * Lx.mul(Ly).mul(Ly).mul(Lxyy) + Ly.mul(Ly).mul(Ly).mul(Lyyy);
}

void ScaleSpaceOpenCVCPU::calcRidge(cv::Mat& Lx, cv::Mat& Ly, cv::Mat& Lxx, cv::Mat& Lxy, cv::Mat& Lyy, cv::Mat& L1, cv::Mat& L2) const
{
  L1 = Lx.mul(Ly).mul(Lxx - Lyy) - (Lx.mul(Lx) - Ly.mul(Ly)).mul(Lxy);
  setLowValuesToZero(L1);

  L2 = (Ly.mul(Ly) - Lx.mul(Lx)).mul(Lxx - Lyy) - 4 * Lx.mul(Ly).mul(Lxy);
}

void ScaleSpaceOpenCVCPU::calcRidgeMax(cv::Mat& L1, cv::Mat& L2, cv::Mat& out) const
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

void ScaleSpaceOpenCVCPU::calcEdgeMax(cv::Mat& L1, cv::Mat& L2, cv::Mat& out) const
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

void ScaleSpaceOpenCVCPU::calcMaxInScale(cv::Mat& L, cv::Mat& out) const
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


void ScaleSpaceOpenCVCPU::processImage(cv::Mat & in, cv::Mat & out, std::function<float (float)> fun) const
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

void ScaleSpaceOpenCVCPU::processImageNonBorder(cv::Mat & in, cv::Mat & out, std::function<unsigned char (cv::Mat&,int,int)> fun) const
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

void ScaleSpaceOpenCVCPU::processTwoImagesNonBorder(cv::Mat & in, cv::Mat & in_sec, cv::Mat & out, std::function<unsigned char (cv::Mat &, cv::Mat &, int, int)> fun) const
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

void ScaleSpaceOpenCVCPU::setLowValuesToZero(cv::Mat& mat) const
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
