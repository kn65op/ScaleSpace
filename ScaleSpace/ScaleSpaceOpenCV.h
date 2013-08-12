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
  unsigned int nr_images;
  
  void doGaussian(ScaleSpaceImage & image);
  
  //pointer to function for different input image
  void (ScaleSpaceOpenCV::* changeToFloat) (cv::Mat & input, cv::Mat &) const;
  
  //functions for different input image
  void changeGrayToFloat(cv::Mat & input, cv::Mat & output) const;
  void changeBayerToFloat(cv::Mat & input, cv::Mat & output) const;

  //pointer to function for mode
  void (ScaleSpaceOpenCV::* doMode) (ScaleSpaceImage & image) const;

  //functions for different modes
  void doPure(ScaleSpaceImage & image) const;
  void doBlob(ScaleSpaceImage & image) const;
  void doEdge(ScaleSpaceImage & image) const;
  void doCorner(ScaleSpaceImage & image) const;
  void doRidge(ScaleSpaceImage & image) const;
  
  //helper functions
  void calcDX(cv::Mat & in, cv::Mat & out) const;
  void calcDY(cv::Mat & in, cv::Mat & out) const;
  void calcDXX(cv::Mat & in, cv::Mat & out) const;
  void calcDXY(cv::Mat & in, cv::Mat & out) const;
  void calcDYY(cv::Mat & in, cv::Mat & out) const;
  void calcDXXX(cv::Mat & in, cv::Mat & out) const;
  void calcDXXY(cv::Mat & in, cv::Mat & out) const;
  void calcDXYY(cv::Mat & in, cv::Mat & out) const;
  void calcDYYY(cv::Mat & in, cv::Mat & out) const;
  //void calcK(cv::Mat & in, cv:Mat & out);

  void calcFirstDeriteratives(cv::Mat & in, cv::Mat &Lx, cv::Mat & Ly) const;
  void calcSecondDeriteratives(cv::Mat & in, cv::Mat &Lxx, cv::Mat & Lxy, cv::Mat & Lyy) const;
  void calcThirdDeriteratives(cv::Mat & in, cv::Mat &Lxxx, cv::Mat & Lxxy, cv::Mat & Lxyy, cv::Mat & Lyyy) const;

};
