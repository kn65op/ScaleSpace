#include "CameraException.h"

using namespace JAI;

CameraException::CameraException(std::string mes/* = ""*/)
{
  message = mes;
}


CameraException::~CameraException(void)
{
}

CameraException::operator std::string()
{
  return message;
}

NoNewFrameException::NoNewFrameException() : CameraException("No new frame")
{
}
