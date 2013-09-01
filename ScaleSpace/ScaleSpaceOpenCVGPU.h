#pragma once
#include "ScaleSpaceOpenCV.h"

class cv::gpu::GpuMat;

class ScaleSpaceOpenCVGPU : public ScaleSpaceOpenCV
{
public:
  /**
   * Contructor which setting mode (ScaleSpaceMode). 
   */
  ScaleSpaceOpenCVGPU(ScaleSpaceMode mode = ScaleSpaceMode::Pure);

  ~ScaleSpaceOpenCVGPU(void);

private:
  void filter2D(cv::Mat & src, cv::Mat &dst, cv::Mat & kernel) const;

  void convertInput(cv::Mat & input, cv::Mat & output) const;
  
  virtual void calcBlob(cv::Mat & Lxx, cv::Mat & Lyy, float sigma, cv::Mat & L) const;
  virtual void calcCorner(cv::Mat & Lx, cv::Mat & Ly, cv::Mat & Lxx, cv::Mat & Lxy, cv::Mat & Lyy, cv::Mat & k) const;
  virtual void calcEdge(cv::Mat & Lx, cv::Mat & Ly, cv::Mat & Lxx, cv::Mat & Lxy, cv::Mat & Lyy, cv::Mat & Lxxx, cv::Mat & Lxxy, cv::Mat & Lxyy, cv::Mat & Lyyy, cv::Mat & L1, cv::Mat & L2) const;
  virtual void calcRidge(cv::Mat &  Lx, cv::Mat & Ly, cv::Mat & Lxx, cv::Mat & Lxy, cv::Mat & Lyy, cv::Mat & L1, cv::Mat & L2) const;

  /*virtual void calcEdgeMax(cv::Mat & L1, cv::Mat & L2, cv::Mat & out) const;
  virtual void calcRidgeMax(cv::Mat & L1, cv::Mat & L2, cv::Mat & out) const;
  virtual void calcMaxInScale(cv::Mat & L, cv::Mat & out) const;
  */
  //void setLowValuesToZero(cv::gpu::GpuMat & in, cv::gpu::GpuMat & out) const;
};

