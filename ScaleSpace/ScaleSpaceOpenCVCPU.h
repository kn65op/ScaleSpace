#pragma once

#include "ScaleSpaceOpenCV.h"

class ScaleSpaceOpenCVCPU : public ScaleSpaceOpenCV
{
public:
  /**
   * Contructor which setting mode (ScaleSpaceMode). If OpenCVGPU doesn't availbe (eg. CUDA is not available) it throws NoPlatformScaleSpaceException.
   */
  ScaleSpaceOpenCVCPU(ScaleSpaceMode mode = ScaleSpaceMode::Pure);

  ~ScaleSpaceOpenCVCPU(void);
  
private:
  void filter2D(cv::Mat & src, cv::Mat &dst, cv::Mat & kernel) const;
  
  void convertInput(cv::Mat & input, cv::Mat & output) const;
  void convertInputFromBayer(cv::Mat & input, cv::Mat & output) const;

  virtual void calcBlob(cv::Mat & Lxx, cv::Mat & Lyy, float sigma, cv::Mat & L) const;
  virtual void calcCorner(cv::Mat & Lx, cv::Mat & Ly, cv::Mat & Lxx, cv::Mat & Lxy, cv::Mat & Lyy, cv::Mat & k) const;
  virtual void calcEdge(cv::Mat & Lx, cv::Mat & Ly, cv::Mat & Lxx, cv::Mat & Lxy, cv::Mat & Lyy, cv::Mat & Lxxx, cv::Mat & Lxxy, cv::Mat & Lxyy, cv::Mat & Lyyy, cv::Mat & L1, cv::Mat & L2) const;
  virtual void calcRidge(cv::Mat &  Lx, cv::Mat & Ly, cv::Mat & Lxx, cv::Mat & Lxy, cv::Mat & Lyy, cv::Mat & L1, cv::Mat & L2) const;
  /*
  virtual void calcEdgeMax(cv::Mat & L1, cv::Mat & L2, cv::Mat & out) const;
  virtual void calcRidgeMax(cv::Mat & L1, cv::Mat & L2, cv::Mat & out) const;
  virtual void calcMaxInScale(cv::Mat & L, cv::Mat & out) const;
  */
  /*
  //functions processing images pixel by pixel
  void setLowValuesToZero(cv::Mat & mat) const;

  //foreach for image
  void processImage(cv::Mat & in, cv::Mat & out, std::function<float (float)> fun) const;
  void processImageNonBorder(cv::Mat & in, cv::Mat & out, std::function<unsigned char (cv::Mat &, int, int)> fun) const;
  void processTwoImagesNonBorder(cv::Mat & in, cv::Mat & in_sec, cv::Mat & out, std::function<unsigned char (cv::Mat &, cv::Mat &, int, int)> fun) const;
  */
};

