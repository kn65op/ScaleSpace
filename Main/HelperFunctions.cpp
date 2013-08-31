#include "HelperFunctions.h"

#include <iostream>
#include <OpenCLDevice.h>

void printCLDeviceInfo()
{
  OpenCLDevice dev = OpenCLDevice::getDevices().front();
  size_t maxw, maxh, maxd, max_work_item_dimensions, max_work_group_size;
  dev.getDeviceInfo(CL_DEVICE_IMAGE3D_MAX_DEPTH, &maxd);
  dev.getDeviceInfo(CL_DEVICE_IMAGE2D_MAX_HEIGHT, &maxh);
  dev.getDeviceInfo(CL_DEVICE_IMAGE2D_MAX_WIDTH, &maxw);
  dev.getDeviceInfo(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, &max_work_item_dimensions);
  dev.getDeviceInfo(CL_DEVICE_MAX_WORK_GROUP_SIZE, &max_work_group_size);
  
  

  std::cout << maxw << " " << maxh << " " << maxd << "\n";
  std::cout << max_work_item_dimensions << "\n";
  std::cout << "Max work group size: " << max_work_group_size << "\n";
}