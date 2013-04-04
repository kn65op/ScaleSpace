#pragma once

#include <opencv\cxcore.h>
#include <string>

#include "ScaleSpaceImage.h"

/**
 * requirements:
 * load image, process, get result
 */

class ScaleSpaceException
{
  std::string message;
public:
  ScaleSpaceException(std::string m) : message(m)
  {
    
  }
  
  operator std::string ()
  {
    return message;
  }
};

/** Scale Space process.
 * It contains all data, which is need to process image with Scale Space algorithm.
 */
class ScaleSpace
{
public:
  /**
   * Default constructor.
   */
  ScaleSpace(void);
  
  /**
   * Destructor.
   */
  ~ScaleSpace(void);

  /** Set range of scales.
   * Scales will be set from 1 (original image) to max with step step.
   * @param max Max scale.
   * @param step Scale step.
   */
  void setScalesRange(unsigned int max, unsigned int step);
  
  /** Set scale step with number of scales.
   * It will create scales from 1 (original image) to nr * step + 1 with step step.
   * @param step Scale step.
   * @param nr Number of scales.
   */
  void setScaleStep(unsigned int step, unsigned int nr);
  
  /** Set max scale with number of scales.
   * It will create scales from 1 (original image) to max with step (max - 1) / nr. If step will be not integer it will throw exception.
   * @param max Max scale.
   * @param nr Number of scales.
   */
  void setMaxScale(unsigned int max, unsigned int nr);
  
  /** Process cv::Mat image.
   * If scales is not set throws ScaleSpaceException.
   * @param input cv::Mat image to process.
   * @param output ScaleSpaceImage with computed representations in specified scales.
   */
  void processImage(cv::Mat & input, ScaleSpaceImage & output);

private:
  unsigned int nr_scales; //number of scales
  unsigned int scale_step; //scale step


};

