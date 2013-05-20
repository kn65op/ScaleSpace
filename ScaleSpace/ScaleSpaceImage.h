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
   * Set image dimensions and number of scales.
   * @param width Image width.
   * @param height Image height.
   * @param scales Number of scales.
   */
  void createImage(unsigned int width, unsigned int height, unsigned int scales, int type);//TODO: temp
  
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
  void show(std::string );
  void show(cv::Mat &blobs, std::vector<float> & sigmas);

  ScaleSpaceImage & operator/=(int d)
  {
    image /= d;
    return *this;
  }
private:
  cv::Mat image;
  
  unsigned int nr_scales;

  unsigned int width;
  unsigned int height;

  int type;//TODO: remove
};

