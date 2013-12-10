#include "Camera.h"
#include "CameraException.h"

#include <cstdlib>
#include <iostream>
#include <opencv2\highgui\highgui.hpp>

using namespace JAI;

#define NODE_NAME_WIDTH         "Width"
#define NODE_NAME_HEIGHT        "Height"
#define NODE_NAME_PIXELFORMAT   "PixelFormat"
#define NODE_NAME_GAIN          "GainRaw"
#define NODE_NAME_ACQSTART      "AcquisitionStart"
#define NODE_NAME_ACQSTOP       "AcquisitionStop"

FACTORY_HANDLE Camera::m_hFactory;

Camera::Camera(int8_t* index)
{
  //copy memory
  memcpy(m_sCameraId, index, J_CAMERA_ID_SIZE);
  m_hCam = NULL;
  m_hThread = NULL;
}


Camera::~Camera(void)
{
  stop();
  close();
  while (queue.size())
  {
    delete queue.front();
    queue.pop();
  }
  while (free_queue.size())
  {
    delete free_queue.front();
    free_queue.pop();
  }
}

void Camera::close() //TODO: Add stop
{
  if (m_hCam)
    {
        // Close camera
        J_Camera_Close(m_hCam);
        m_hCam = NULL;
    }

    if (m_hFactory)
    {
        // Close factory
        J_Factory_Close(m_hFactory);
        m_hFactory = NULL;
    }
}

bool Camera::open(OutputMode mode)
{
  J_STATUS_TYPE   retval;

  // Open camera
  retval = J_Camera_Open(m_hFactory, m_sCameraId, &m_hCam);
  if (retval != J_ST_SUCCESS)
  {
    //TODO:throw error
    //AfxMessageBox(CString("Could not open the camera!"), MB_OK | MB_ICONEXCLAMATION);
    return FALSE;
  }
  std::cout << "Opening camera succeeded\n";
  //set
  mode_set = mode;
  switch(mode)
  {
  case OutputMode::UNIT_8:
    retval = J_Camera_SetValueInt64(m_hCam, reinterpret_cast<int8_t*>(NODE_NAME_PIXELFORMAT), J_GVSP_PIX_BAYGB8);
    break;
  case OutputMode::UNIT_10:
    retval = J_Camera_SetValueInt64(m_hCam, reinterpret_cast<int8_t*>(NODE_NAME_PIXELFORMAT), J_GVSP_PIX_BAYGB10);
    break;
  case OutputMode::UNIT_12:
    retval = J_Camera_SetValueInt64(m_hCam, reinterpret_cast<int8_t*>(NODE_NAME_PIXELFORMAT), J_GVSP_PIX_BAYGB12);
    break;
  case OutputMode::UNIT_16:
    retval = J_Camera_SetValueInt64(m_hCam, reinterpret_cast<int8_t*>(NODE_NAME_PIXELFORMAT), J_GVSP_PIX_BAYGB16);
    break;
  default:
    throw CameraException("Somothing went wrong");
  }
  tGenICamErrorInfo info;
  J_Factory_GetGenICamErrorInfo(&info);
std::cout << info.sDescription << "\n"<< info.sDescription << "\n";

  return TRUE;
}

bool Camera::checkIfJaiSdkIsInstalled()
{
  char * jai_env = getenv("JAI_SDK_BIN");
  return jai_env;
}

std::list<Camera*> Camera::getCameraList()
{
  std::list<Camera*> ret; //TODO: autoptr
  J_STATUS_TYPE   retval;
  uint32_t        iSize;
  uint32_t        iNumDev;
  bool8_t         bHasChange;

  std::cout << J_REG_DATABASE << "\n";
  
  if (!checkIfJaiSdkIsInstalled())
  {
    throw CameraException("There is no JAISDK installed or it is broken!");
  }
  
  retval = J_Factory_Open(reinterpret_cast<const int8_t*>(J_REG_DATABASE), &m_hFactory);
  
  if (retval != J_ST_SUCCESS)
  {
    throw CameraException("Could not open factory!");
  }
  std::cout << "Opening factory succeeded\n";

  // Update camera list
  retval = J_Factory_UpdateCameraList(m_hFactory, &bHasChange);
  if (retval != J_ST_SUCCESS)
  {
    throw CameraException("Could not update camera list!");
  }
  std::cout << "Updating camera list succeeded\n";

  // Get the number of Cameras
  retval = J_Factory_GetNumOfCameras(m_hFactory, &iNumDev);
  if (retval != J_ST_SUCCESS)
  {
    throw CameraException("Could not get the number of cameras!");
  }
  if (iNumDev == 0)
  {
    throw CameraException("There is no camera!");
  }
  std::cout << "%d cameras were found" << iNumDev << "\n";

  // Get camera ID
  int8_t m_sCameraId[J_CAMERA_ID_SIZE];
  for (uint32_t i=0; i<iNumDev; ++i)
  {
    iSize = (uint32_t)sizeof(m_sCameraId);
    retval = J_Factory_GetCameraIDByIndex(m_hFactory, i, m_sCameraId, &iSize);
    if (retval != J_ST_SUCCESS)
    {
      throw CameraException("Could not get the camera ID!");
    }
    ret.push_back(new Camera(m_sCameraId));
  }
  std::cout << "Camera ID: %s\n" << m_sCameraId << "\n";
  return ret;
}

bool Camera::start(OutputMode mode)
{
  J_STATUS_TYPE   retval;
  int64_t int64Val;
  int64_t pixelFormat;

  SIZE	ViewSize;
  POINT	TopLeft;

  // Get Width from the camera
  retval = J_Camera_GetValueInt64(m_hCam, reinterpret_cast<int8_t*>(NODE_NAME_WIDTH), &int64Val);
  ViewSize.cx = (LONG)int64Val;     // Set window size cx

  // Get Height from the camera
  retval = J_Camera_GetValueInt64(m_hCam, reinterpret_cast<int8_t*>(NODE_NAME_HEIGHT), &int64Val);
  ViewSize.cy = (LONG)int64Val;     // Set window size cy

  //set
  mode_set = mode;
  switch(mode)
  {
  case OutputMode::UNIT_8:
    retval = J_Camera_SetValueInt64(m_hCam, reinterpret_cast<int8_t*>(NODE_NAME_PIXELFORMAT), J_GVSP_PIX_BAYGB8);
    break;
  case OutputMode::UNIT_10:
    retval = J_Camera_SetValueInt64(m_hCam, reinterpret_cast<int8_t*>(NODE_NAME_PIXELFORMAT), J_GVSP_PIX_BAYGB10);
    break;
  case OutputMode::UNIT_12:
    retval = J_Camera_SetValueInt64(m_hCam, reinterpret_cast<int8_t*>(NODE_NAME_PIXELFORMAT), J_GVSP_PIX_BAYGB12);
    break;
  case OutputMode::UNIT_16:
    retval = J_Camera_SetValueInt64(m_hCam, reinterpret_cast<int8_t*>(NODE_NAME_PIXELFORMAT), J_GVSP_PIX_BAYGB16);
    break;
  default:
    throw CameraException("Somothing went wrong");
  }
  //J_GVSP_PIX_BAYBG8
  //J_GVSP_PIX_BAYRG10
  //J_GVSP_PIX_BAYRG12
  //J_GVSP_PIX_BAYGR16



  // Get pixelformat from the camera
  retval = J_Camera_GetValueInt64(m_hCam, reinterpret_cast<int8_t*>(NODE_NAME_PIXELFORMAT), &int64Val);
  pixelFormat = int64Val;

  // Calculate number of bits (not bytes) per pixel using macro
  int bpp = J_BitsPerPixel(pixelFormat);

  // Set window position
  TopLeft.x = 100;
  TopLeft.y = 50;

  // Open stream
  retval = J_Image_OpenStream(m_hCam, 0, reinterpret_cast<J_IMG_CALLBACK_OBJECT>(this), reinterpret_cast<J_IMG_CALLBACK_FUNCTION>(&Camera::callback), &m_hThread, (ViewSize.cx*ViewSize.cy*bpp)/8);
  if (retval != J_ST_SUCCESS) {
    return false;
  }
  std::cout << "Opening stream succeeded\n";

  // Start Acquision
  retval = J_Camera_ExecuteCommand(m_hCam, reinterpret_cast<int8_t*>(NODE_NAME_ACQSTART));

  return true;
}

void Camera::stop()
{
  J_STATUS_TYPE retval;

  // Stop Acquision
  if (m_hCam) {
    retval = J_Camera_ExecuteCommand(m_hCam, reinterpret_cast<int8_t*>(NODE_NAME_ACQSTOP));
  }

  if(m_hThread)
  {
    // Close stream
    retval = J_Image_CloseStream(m_hThread);
    m_hThread = NULL;
    std::cout << "Closed stream\n";
  }
}

void Camera::callback(J_tIMAGE_INFO * pAqImageInfo)
{
  if (queue.size() == queue_size) //max limit for queue 
  {
    queue_mutex.lock();
    free_queue.push(queue.front());
    queue.pop();
    queue_mutex.unlock();
    std::cout << "Warning: Oldest frame was dropeed\n";
  }
  cv::Mat * tmp;
  if (free_queue.empty()) //get new image
  {
    if (mode_set == OutputMode::UNIT_8)
    {
      tmp = new cv::Mat(pAqImageInfo->iSizeY, pAqImageInfo->iSizeX, CV_8UC1);
    }
    else
    {
      tmp = new cv::Mat(pAqImageInfo->iSizeY, pAqImageInfo->iSizeX, CV_16UC1);
    }
  }
  else //get image from free_queue
  {
    tmp = free_queue.front();
    free_queue.pop();
  }
  memcpy(tmp->data, pAqImageInfo->pImageBuffer, tmp->total() * tmp->elemSize());
  queue_mutex.lock();
  queue.push(tmp);
  queue_mutex.unlock();
}

cv::Mat Camera::getNextFrame()
{
  if (queue.empty())
  {
    throw NoNewFrameException();
  }
  queue_mutex.lock();
  cv::Mat * ret = queue.front();
  queue.pop();
  queue_mutex.unlock();
  free_queue.push(ret);
  //std::cout << queue.size() << "Queue size\n";
  return *ret;
}

void Camera::getImageSize(int & x, int & y)
{
  if (!m_hCam)
  {
    throw CameraException("No handle for camera - you must open camera first");
  }
  J_STATUS_TYPE   retval;
  int64_t int64Val;
  retval = J_Camera_GetValueInt64(m_hCam, reinterpret_cast<int8_t*>(NODE_NAME_WIDTH), &int64Val);
  x = (int)int64Val;     // Set window size cx

  // Get Height from the camera
  retval = J_Camera_GetValueInt64(m_hCam, reinterpret_cast<int8_t*>(NODE_NAME_HEIGHT), &int64Val);
  y = (int)int64Val;     // Set window size cy
}