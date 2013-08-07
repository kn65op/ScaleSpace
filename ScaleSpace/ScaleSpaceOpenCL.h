#pragma once

#include <opencv\cxcore.h>
#include <string>
#include <vector>
#include <OpenCLAlgorithmsStream.h>
#include <OpenCL3DTo2DImageAlgorithm.h>

#include "ScaleSpace.h"

/**
 * requirements:
 * load image, process (Gaussian and optionally other operators [laplacian]), get result
 */



/** Scale Space process.
 * It contains all data, which is need to process image with Scale Space algorithm.
 * Currently step can be only even.
 */
class ScaleSpaceOpenCL : public ScaleSpace
{
public:
  /**
   * Contructor which setting mode (ScaleSpaceMode).
   */
  ScaleSpaceOpenCL(ScaleSpaceMode mode = ScaleSpaceMode::Pure);
  
  /**
   * Destructor.
   */
  ~ScaleSpaceOpenCL(void);
  
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
  typedef std::list<OpenCLAlgorithmsStream*> streams_t;
  streams_t streams; //streams - one for each scale

  OpenCLImageCommon* post_processing;
  
  //clearing streams
  void clearStreams();

  //information about last processed image (to avoid alocating and dealocating memory etc.)
  unsigned int last_height;
  unsigned int last_width;
  unsigned int last_scale;

  ScaleSpaceMode calc_mode;
  ScaleSpaceOutputType output_type;

  //sigmas
  std::vector<float> sigmas;

  int type;//TODO: tmp

};

