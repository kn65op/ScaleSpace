#pragma once

#include "ScaleSpace.h"

#include <functional>

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

  //pointer to postprocessing function
  void (ScaleSpaceOpenCV::* doPostProcessing) (ScaleSpaceImage & image) const;

  //postprocessing functions
  void findMaxInScale(ScaleSpaceImage & image) const;
  void findEdgeMax(ScaleSpaceImage & image) const;
  void findRidgeMax(ScaleSpaceImage & image) const;
  
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

  void calcFirstDeriteratives(cv::Mat & in, cv::Mat &Lx, cv::Mat & Ly) const;
  void calcSecondDeriteratives(cv::Mat & in, cv::Mat &Lxx, cv::Mat & Lxy, cv::Mat & Lyy) const;
  void calcThirdDeriteratives(cv::Mat & in, cv::Mat &Lxxx, cv::Mat & Lxxy, cv::Mat & Lxyy, cv::Mat & Lyyy) const;

  //foreach for image
  void processImage(cv::Mat & in, cv::Mat & out, std::function<float (float)> fun) const;
  void processImageNonBorder(cv::Mat & in, cv::Mat & out, std::function<unsigned char (cv::Mat &, int, int)> fun) const;
  void processTwoImagesNonBorder(cv::Mat & in, cv::Mat & in_sec, cv::Mat & out, std::function<unsigned char (cv::Mat &, cv::Mat &, int, int)> fun) const;
  
  //functions processing images pixel by pixel
  void setLowValuesToZero(cv::Mat & mat) const;

protected:


};
