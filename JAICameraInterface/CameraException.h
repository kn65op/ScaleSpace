#pragma once

#include <string>

namespace JAI
{

/**
 * Camera exception.
 */
class CameraException
{
public:
  CameraException(std::string mes = "");
  ~CameraException(void);

  operator std::string ();
private:
  std::string message;
};

class NoNewFrameException : public CameraException
{
public:
  NoNewFrameException();
};

}