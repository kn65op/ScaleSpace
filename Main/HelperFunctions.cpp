#include "HelperFunctions.h"

#include <iostream>
#include <OpenCLDevice.h>

void printCLDeviceInfo()
{
  OpenCLDevice dev = OpenCLDevice::getDevices().front();
  size_t maxw, maxh, maxd;
  dev.getDeviceInfo(CL_DEVICE_IMAGE3D_MAX_DEPTH, &maxd);
  dev.getDeviceInfo(CL_DEVICE_IMAGE2D_MAX_HEIGHT, &maxh);
  dev.getDeviceInfo(CL_DEVICE_IMAGE2D_MAX_WIDTH, &maxw);

  std::cout << maxw << " " << maxh << " " << maxd << "\n";
}