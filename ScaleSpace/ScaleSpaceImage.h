#pragma once

#include <opencv\cxcore.h>

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
   * Set image dimensions and number of scales.
   * @param width Image width.
   * @param height Image height.
   * @param scales Number of scales.
   */
  void createImage(unsigned int width, unsigned int height, unsigned int scales);
  
  /**
   * Set original image.
   * @param image Original image.
   */
  void setOriginalImage(cv::Mat original_image);
  
  /**
   * Get image data for image in specified scale.
   * @param scale Scale number.
   * @return Image data.
   */
  void * getDataForScale(unsigned int scale);
  void show();
private:
  cv::Mat image;
  
  unsigned int nr_scales;

  unsigned int width;
  unsigned int height;
};

