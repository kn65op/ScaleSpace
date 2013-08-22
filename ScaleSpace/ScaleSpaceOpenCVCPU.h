#pragma once

#include "ScaleSpaceOpenCV.h"

class ScaleSpaceOpenCVCPU : public ScaleSpaceOpenCV
{
public:
  /**
   * Contructor which setting mode (ScaleSpaceMode). 
   */
  ScaleSpaceOpenCVCPU(ScaleSpaceMode mode = ScaleSpaceMode::Pure);

  ~ScaleSpaceOpenCVCPU(void);
  
private:
  void filter2D(cv::Mat & src, cv::Mat &dst, cv::Mat & kernel) const;
  
  virtual void calcBlob(cv::Mat & Lxx, cv::Mat & Lyy, float sigma, cv::Mat & L) const;
  virtual void calcCorner(cv::Mat & Lx, cv::Mat & Ly, cv::Mat & Lxx, cv::Mat & Lxy, cv::Mat & Lyy, cv::Mat & k) const;
  virtual void calcEdge(cv::Mat & Lx, cv::Mat & Ly, cv::Mat & Lxx, cv::Mat & Lxy, cv::Mat & Lyy, cv::Mat & Lxxx, cv::Mat & Lxxy, cv::Mat & Lxyy, cv::Mat & Lyyy, cv::Mat & L1, cv::Mat & L2) const;
  virtual void calcRidge(cv::Mat &  Lx, cv::Mat & Ly, cv::Mat & Lxx, cv::Mat & Lxy, cv::Mat & Lyy, cv::Mat & L1, cv::Mat & L2) const;
};

