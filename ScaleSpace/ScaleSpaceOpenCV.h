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
  void (ScaleSpaceOpenCV::* changeToFloat) (cv::Mat &, cv::Mat &) const;
  
  //functions for different input image
  void changeGrayToFloat(cv::Mat & input, cv::Mat & output) const;
  void changeBayerToFloat(cv::Mat & input, cv::Mat & output) const;

  //pointer to function for mode
  void (ScaleSpaceOpenCV::* doMode) (cv::Mat &, ScaleSpaceImage & image) const;

  //functions for different modes
  void doPure(cv::Mat &, ScaleSpaceImage & image) const;
  void doBlob(cv::Mat &, ScaleSpaceImage & image) const;
  void doEdge(cv::Mat &, ScaleSpaceImage & image) const;
  void doCorner(cv::Mat &, ScaleSpaceImage & image) const;
  void doRidge(cv::Mat &, ScaleSpaceImage & image) const;
  
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

};
