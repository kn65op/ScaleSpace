#pragma once

#include <list>
#include <queue>
#include <mutex>

#include <opencv2/core/core.hpp>

#include <Jai_Factory.h>

namespace JAI
{

enum class OutputMode
{
  UNIT_8,
  UNIT_10,
  UNIT_12,
  UNIT_16
};


/**
 * @brief Class implement JAI Camera interface.
 * 
 * Example of usage:
 * JAI::Camera *camera = JAI::Camera::getCameraList().front();
 * camera.open(); //check return value
 * camera.start(); //check return value
 * //in loop, with try and catch blocks in case of getting new image, when camera haven't return it yet.
 * cv::Mat image = camera.getNextFrame();
 * //do stuff with image
 * camera.stop(); 
 * camera.close();
 * @param index
 */
class Camera
{
public:
  Camera(int8_t* index);
  ~Camera(void);

  /**
   * Open camera.
   * @return true if opening was successful.
   */
  bool open(OutputMode mode = OutputMode::UNIT_8);
  /**
   * Close camera.
   */
  void close();

  /**
   * Start capturing images.
   * @return true if opening was successful.
   */
  bool start(OutputMode mode = OutputMode::UNIT_8);
  /**
   * Stop capturing images.
   */
  void stop();

  /**
   * Get next captured image. 
   * @return New image. If no new image have been captured throw NoNewFrameException.
   */
  cv::Mat getNextFrame();

  /**
   * Get camera image size.
   * @param x Width of image.
   * @param y Height of image.
   */
  void getImageSize(int & x, int & y);

  /**
   * Get list of all cameras.
   * @return List of all cameras.
   */
  static std::list<Camera*> getCameraList();

private:
  static bool checkIfJaiSdkIsInstalled();

  int index;

  //JAI specific objects
  static FACTORY_HANDLE  m_hFactory;     // Factory Handle
  CAM_HANDLE      m_hCam;         // Camera Handle
  THRD_HANDLE     m_hThread;      // Acquisition Thread Handle
  int8_t          m_sCameraId[J_CAMERA_ID_SIZE];    // Camera ID
  CvMat           *m_pMapMatrix;  // OpenCV Matrix

  //max size of queue
  static const unsigned int queue_size = 20;

  std::queue<cv::Mat*> queue;
  std::queue<cv::Mat*> free_queue;

  std::mutex queue_mutex;

  void callback(J_tIMAGE_INFO * pAqImageInf);

  OutputMode mode_set;


};

}
