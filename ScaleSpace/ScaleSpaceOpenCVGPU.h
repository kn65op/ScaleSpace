#pragma once
#include "ScaleSpace.h"

class ScaleSpaceOpenCVGPU : public ScaleSpace
{
public:
  /**
   * Contructor which setting mode (ScaleSpaceMode). 
   */
  ScaleSpaceOpenCVGPU(ScaleSpaceMode mode = ScaleSpaceMode::Pure);

  ~ScaleSpaceOpenCVGPU(void);

  /** Process cv::Mat image.
   * If ScaleSpace is not prepared it will be. If error occure during preparation will throw ScaleSpaceException.
   * @param input cv::Mat image to process.
   * @param output ScaleSpaceImage with computed representations in specified scales.
   */
  void processImage(cv::Mat & input, ScaleSpaceImage & output);

  /**
   * Prepare stream for computing.
   */
  void prepare(ScaleSpaceSourceImageType si_type, ScaleSpaceOutputType out_type = ScaleSpaceOutputType::ONE_IMAGE);
};

