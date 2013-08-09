#pragma once

#include "ScaleSpace.h"

class ScaleSpaceOpenCV : public ScaleSpace
{
public:
  /**
   * Contructor which setting mode (ScaleSpaceMode). 
   */
  ScaleSpaceOpenCV(ScaleSpaceMode mode = ScaleSpaceMode::Pure);

  ~ScaleSpaceOpenCV(void);

  /** Process cv::Mat image.
   * If ScaleSpace is not prepared it will be. If error occure during preparation will throw ScaleSpaceException.
   * @param output ScaleSpaceImage with input image and computed representations in specified scales.
   */
  virtual void processImage(ScaleSpaceImage & image);

  /**
   * Prepare stream for computing.
   */
  void prepare(ScaleSpaceSourceImageType si_type, ScaleSpaceOutputType out_type = ScaleSpaceOutputType::ONE_IMAGE);

private:
  int temp_image_type;
  
  void doGaussian(ScaleSpaceImage & image);
  
  //pointer to function for different input image
  void (ScaleSpaceOpenCV::* changeToFloat) (cv::Mat & input, cv::Mat &) const;
  
  //functions for different input image
  void changeGrayToFloat(cv::Mat & input, cv::Mat & output) const;
  void changeBayerToFloat(cv::Mat & input, cv::Mat & output) const;

  //pointer to function for mode
  void (ScaleSpaceOpenCV::* doMode) (cv::Mat & input, ScaleSpaceImage & image) const;

  //functions for different modes
  void doPure(cv::Mat & input, ScaleSpaceImage & image) const;
  void doBlob(cv::Mat & input, ScaleSpaceImage & image) const;
  void doEdge(cv::Mat & input, ScaleSpaceImage & image) const;
  void doCorner(cv::Mat & input, ScaleSpaceImage & image) const;
  void doRidge(cv::Mat & input, ScaleSpaceImage & image) const;
  
  //helper functions
  void calcDX(cv::Mat & in, cv::Mat & out);
  void calcDY(cv::Mat & in, cv::Mat & out);
  void calcDXX(cv::Mat & in, cv::Mat & out);
  void calcDXY(cv::Mat & in, cv::Mat & out);
  void calcDYY(cv::Mat & in, cv::Mat & out);
  void calcDXXX(cv::Mat & in, cv::Mat & out);
  void calcDXXY(cv::Mat & in, cv::Mat & out);
  void calcDXYY(cv::Mat & in, cv::Mat & out);
  void calcDYYY(cv::Mat & in, cv::Mat & out);
  //void calcK(cv::Mat & in, cv:Mat & out);

  void calcFirstDeriteratives(cv::Mat & in, cv::Mat &Lx, cv::Mat & Ly);
  void calcSecondDeriteratives(cv::Mat & in, cv::Mat &Lxx, cv::Mat & Lxy, cv::Mat & Lyy);
  void calcThirdDeriteratives(cv::Mat & in, cv::Mat &Lxxx, cv::Mat & Lxxy, cv::Mat & Lxyy, cv::Mat & Lyyy);

};
