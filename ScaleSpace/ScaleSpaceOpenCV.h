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
  void (ScaleSpaceOpenCV::* changeToFloat) (cv::Mat &, cv::Mat &) const;
  
  void changeGrayToFloat(cv::Mat & input, cv::Mat & output) const;
  void changeBayerToFloat(cv::Mat & input, cv::Mat & output) const;
};
