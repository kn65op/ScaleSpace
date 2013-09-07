#include "HelperFunctions.h"

#include <iostream>
#include <OpenCLDevice.h>

using namespace TTime;

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

void processScaleSpace(ScaleSpace *ss, ProgramController & controller, cv::Mat & input, ScaleSpaceImage & output, Stoper & stoper, bool show, unsigned int image_number, bool calc_first_image)
{
  output.setInput(input);
  if (image_number != 0 || calc_first_image)
  {
    stoper.start(false);
  }
  ss->processImage(output, image_number == 0);
  try
  {
    stoper.stop();
  }
  catch (NotStartedStoperException ex)
  {
    if (image_number != 0 && calc_first_image)
    {
      throw ex;
    }
  }
  if (show)
  {
    output.show(controller.getOutputPrefix(), getStringFromScaleSpaceProcessor(controller.getProcessor()), image_number);
  }
}

std::string getFileWithPrefix(std::string prefix, unsigned int i)
{
  return prefix + "_" + (i < 10 ? "0" + std::to_string(i) : std::to_string(i)) + ".bmp";
}

