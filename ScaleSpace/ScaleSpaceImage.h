#pragma once

#include <opencv\cxcore.h>

#include <vector>

class ScaleSpaceImageException
{
  std::string message;
public:
  ScaleSpaceImageException(std::string m) : message(m)
  {
    
  }
  
  operator std::string ()
  {
    return message;
  }
};

/** Class represents 3D image created with scale space algorithm.
 * 
 */
class ScaleSpaceImage
{
public:
  ScaleSpaceImage(void);
  ~ScaleSpaceImage(void);

  /**
   * Set image dimensions, number of scales, type and number of required temporary images.
   * @param scales Number of scales.
   * @param type Type of image (in OpenCV way).
   * @param images Number of required temporary images. Default is 1.
   */
  void createImage(unsigned int scales, int type, unsigned int images = 1);//TODO: temp
  
  /**
   * Set input image.
   * @param image Input image.
   */
  void setInput(cv::Mat image);
  
  /**
   * Get image data for image in specified scale.
   * @param scale Scale number.
   * @param image_number Image number for scale. Default is 0. Some algorithms has to provide two images durig processing.
   * @return Pointer to image data.
   */
  void * getDataForScale(unsigned int scale, unsigned int image_number = 0);
  
  /**
   * Get image for scale.
   * @param scale Scale number.1
   * @param image_number Image number for scale. Default is 0. Some algorithms has to provide two images durig processing.
   * @return cv::Mat with proper image.
   */
  cv::Mat & getImageForScale(unsigned int scale, unsigned int image_number = 0);

  /**
   * Get pointer to data for output image.
   * @param scale Image scale.
   * @return Pointer to data.
   */
  void * getDataForOutput(unsigned int scale);
  
  /**
   * Get output image for scale.
   * @param scale Image scale.
   * @return cv::Mat with image.
   */
  cv::Mat & getOutput(unsigned int scale);
  
  /**
   * Returns input image size.
   * @return cv::Size
   */
  cv::Size size() const;
  
  /**
   * Returns input image data.
   * @return void pointer to image data.
   */
  void * getInputData() const;
  
  /**
   * Returns input image type.
   * @return Input image type in terms of OpenCV.
   */
  int type() const;
  
  /**
   * Get Input image.
   * @return cv::Mat with input image.
   */
  cv::Mat & getInput();

  void show(std::string prefix, std::string processor, unsigned int image_number = 0);
  void show(cv::Mat &blobs, std::vector<float> & sigmas);

  unsigned int getOneImageSize() const;

  ScaleSpaceImage & operator/=(int d)
  {
    for (auto image : scale_space_images[0])
    {
      image /= d;
    }
    return *this;
  }
private:
  typedef std::vector<cv::Mat> vector_mat_t;
  std::vector<vector_mat_t> scale_space_images;
  
  vector_mat_t output;
  
  cv::Mat input;
  
  unsigned int nr_scales;
  unsigned int nr_images;

  int temporary_image_type;//TODO: remove
  
  void checkScale(unsigned int scale);
};

