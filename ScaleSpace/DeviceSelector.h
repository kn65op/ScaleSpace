#pragma once

#include <string>
#include <OpenCLDevice.h>



class DeviceSelector
{
public:
  DeviceSelector(void);
  ~DeviceSelector(void);

  enum class Platform
  {
    NVIDIA,
    INTEL,
    NONE
  };

  class WrongPlatorfmException
  {
  };
  class NotFoundDeviceException
  {
  };

  static OpenCLDevice selectDevice(std::string platform_name);
  static std::string getOpenCLPlatformString(Platform platform);
  static Platform getPlatformFromUserString(std::string user);
};

