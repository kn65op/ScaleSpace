#pragma once

#include "ScaleSpaceImage.h"

/**
 * Scale Space Exception class.
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

class ScaleSpaceZeroException : public ScaleSpaceException
{
public:
  ScaleSpaceZeroException(std::string m) : ScaleSpaceException(m)
  {
    
  }
};


class NoPlatformScaleSpaceException : public ScaleSpaceException
{
public:
  NoPlatformScaleSpaceException(std::string m) : ScaleSpaceException(m)
  {
  }
};

/** Enum for selecting what processing will be done.
 * Supproted values:
 * - Pure - means that only SpaleSpace representation will be calculed.
 * - Laplacian - means that after ScaleSpace representation Lapalacian will be calculted, which can detect blobs.
 * - Edges - find edges on image.
 */
enum class ScaleSpaceMode
{
  Pure,
  Laplacian,
  Edges,
  Blobs,
  Corners,
  Ridges
};

enum class ScaleSpaceSourceImageType
{
  Gray,
  Bayer
};

enum class ScaleSpaceOutputType
{
  ONE_IMAGE,
  IMAGE_FOR_SCALE
};

enum class ScaleSpaceProcessor
{
  OPENCL,
  OPENCV_CPU,
  OPENCV_GPU
};

std::ostream & operator<<(std::ostream& out, ScaleSpaceProcessor processor);
std::ostream & operator<<(std::ostream& out, ScaleSpaceMode mode);
std::ostream & operator<<(std::ostream& out, ScaleSpaceSourceImageType type);

std::string getStringFromScaleSpaceProcessor(const ScaleSpaceProcessor & processor);

struct ScaleSpaceParameters
{
  bool quiet;
  bool calc_first_image;
  bool debug;
};

/** Scale Space process.
 * It contains all data, which is need to process image with Scale Space algorithm.
 * Currently step can be only even.
 */
class ScaleSpace
{
public:
  /**
   * Contructor which setting mode (ScaleSpaceMode). 
   */
  ScaleSpace(ScaleSpaceMode mode = ScaleSpaceMode::Pure);
  
  /**
   * Destructor.
   */
  virtual ~ScaleSpace(void);

  /** Set range of scales.
   * Scales will be set from 1 (original image) to max with step step.
   * @param max Max scale. Has to be more then 0, otherwise throws ScaleSpaceException.
   * @param step Scale step which has to be even and not 0, otherwise throws ScaleSpaceException.
   * @return true if max is part of scales selection, false otherwise.
   */
  bool setScalesRange(unsigned int max, unsigned int step);
  
  /** Set scale step with number of scales.
   * It will create scales from 1 (original image) to nr * step + 1 with step step.
   * @param step Scale step which has to be even and not 0, otherwise throws ScaleSpaceException.
   * @param nr Number of scales. Has to be more then 0, otherwise throws ScaleSpaceException
   */
  void setScaleStep(unsigned int step, unsigned int nr);
  
  /** Set max scale with number of scales.
   * It will create scales from 1 (original image) to max with step (max - 1) / nr. If step will be not even integer it will throw exception.
   * @param max Max scale. Has to be more then 0, otherwise throws ScaleSpaceException.
   * @param nr Number of scales. Has to be more then 0, otherwise throws ScaleSpaceException
   */
  void setMaxScale(unsigned int max, unsigned int nr);
  
  /** Process cv::Mat image.
   * If ScaleSpace is not prepared it will be. If error occure during preparation will throw ScaleSpaceException.
   * @param output ScaleSpaceImage with input image and computed representations in specified scales.
   */
  virtual void processImage(ScaleSpaceImage & image, bool first_image);
  
  /**
   * Reutnrs unsigned int with number of scales.
   * @return Number of scales.
   */
  unsigned int getNrScales();
  
  /**
   * Reutnrs unsigned int with scale step.
   * @return Scale step.
   */
  unsigned int getScaleStep();
  
  /**
   * Prepare stream for computing.
   */
  virtual void prepare(ScaleSpaceSourceImageType si_type, ScaleSpaceOutputType out_type = ScaleSpaceOutputType::ONE_IMAGE) = 0;
  
  /**
   * Return gaussian for selected scale.
   * @param scale Scale for which gaussian will be created.
   * @return cv::Mat with gaussian.
   */
  cv::Mat getGaussianForScale(unsigned int scale);

  /**
   * Return gaussian size for selected scale.
   * @param scale Scale for which gaussian size will be computed.
   * @return unsigned int with gaussian size.
   */
  unsigned int getGaussianSizeForScale(unsigned int scale);

  void setParameters(ScaleSpaceParameters par);

protected:
  unsigned int nr_scales; //number of scales
  unsigned int scale_step; //scale step
  
  ScaleSpaceParameters parameters;

  bool prepared; //if streams is prepared
  
  //sigmas
  std::vector<float> sigmas;

  ScaleSpaceMode calc_mode;

  virtual void process(ScaleSpaceImage & image, bool first_image) = 0;
  
private:
  void makeSigmas();
};

