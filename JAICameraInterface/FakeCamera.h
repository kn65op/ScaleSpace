#pragma once

#include<list>
#include<queue>

#include <opencv2/core/core.hpp>

#include <Jai_Factory.h>

namespace JAI
{

/**
 * Fake camera that imitates real. It reads files and returns it like a camera.
 * @param index
 */
class FakeCamera
{
public:
  FakeCamera(int8_t* index);
  ~FakeCamera(void);

  //open/close
  bool open();
  void close();

  bool start();
  void stop();

  cv::Mat getNextFrame();

  cv::Mat getImageSize(int & x, int & y);
  //void setCallbackFunction(

  static std::list<FakeCamera*> getCameraList();

  void setDir(std::string d);
  void setPrefix(std::string prefix);

private:
  std::string name_prefix;
  std::string name_postfix;
  int d;
  int u;
  int h;
  std::string dir;
};

}