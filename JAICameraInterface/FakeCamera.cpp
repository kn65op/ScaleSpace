#include "FakeCamera.h"
#include "CameraException.h"

#include <iostream>
#include <sstream>

#include <opencv/highgui.h>

using namespace JAI;

#define NODE_NAME_WIDTH         "Width"
#define NODE_NAME_HEIGHT        "Height"
#define NODE_NAME_PIXELFORMAT   "PixelFormat"
#define NODE_NAME_GAIN          "GainRaw"
#define NODE_NAME_ACQSTART      "AcquisitionStart"
#define NODE_NAME_ACQSTOP       "AcquisitionStop"

FakeCamera::FakeCamera(int8_t* index)
{
//  name_prefix = "RecordedImage_BB-500GE_00-0C-DF-04-11-C3_";
  name_postfix = ".bmp";
  d = 0;
  u = 0;
  h = 0;
  //dir = "../../data2/";
}


FakeCamera::~FakeCamera(void)
{
}

void FakeCamera::close()
{
}

bool FakeCamera::open()
{
  return true;
}

std::list<FakeCamera*> FakeCamera::getCameraList()
{
  std::list<FakeCamera*> ret;
  ret.push_back(new FakeCamera(NULL));
  return ret;
}

bool FakeCamera::start()
{
  return true;
}

void FakeCamera::stop()
{
}

cv::Mat FakeCamera::getNextFrame()
{
  std::stringstream filename;
  filename << dir << name_prefix << h << d << u << name_postfix;
  if (++u == 10)
  {
    u = 0;
    if(++d == 10)
    {
      d = 0;
      ++h;
    }
  }
  cv::Mat ret = cv::imread(filename.str(), -1);
  if (ret.size().height == 0)
  {
    throw NoNewFrameException();
  }
  return ret;
}

cv::Mat FakeCamera::getImageSize(int & x, int & y)
{
  std::stringstream filename;
  filename << dir << name_prefix << h << d << u << name_postfix;
  cv::Mat ret = cv::imread(filename.str(), -1);
  x = ret.size().width;
  y = ret.size().height;
  return ret;
}

void FakeCamera::setDir(std::string d)
{
  dir = d;
}

void FakeCamera::setPrefix(std::string prefix)
{
  name_prefix = prefix;
}