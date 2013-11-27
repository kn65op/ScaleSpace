#include "DeviceSelector.h"
#include <iostream>

DeviceSelector::DeviceSelector(void)
{
}


DeviceSelector::~DeviceSelector(void)
{
}

OpenCLDevice DeviceSelector::selectDevice(std::string platform_name)
{
  OpenCLDevice device;
  auto dev_list = OpenCLDevice::getDevices();
  if (platform_name != "") //specified platform name - selecting first
  {
    for (OpenCLDevice d: dev_list)
    {
      if (d.getPlatformName() == platform_name)
      {
        device = d;
        break;
      }
    }
    if (!device.isValid())
    {
      throw NotFoundDeviceException();
    }
  }
  else
  {
    bool notok = true;
    while (notok)
    {
      std::cout << "Choose your device - select proper number:\n";
      int i = 1;
      for (OpenCLDevice d : dev_list)
      {
        std::cout << i++ << ": " << d.getPlatformName() << " " << d.getName() << "\n";
      }
      std::string choosen_str;
      std::cin >> choosen_str;
      int choosen = atoi(choosen_str.c_str());
      if (choosen > 0 &&  choosen < i)
      {
        auto it = dev_list.begin();
        for (int i=1; i<choosen; ++i)
        {
          ++it;
        }
        device = *it;
        notok = false;
      }
      else
      {
        std::cout << "Wrong number, select one more time\n";
      }
    }
  }
  return device;
}

std::string DeviceSelector::getOpenCLPlatformString(DeviceSelector::Platform platform)
{
  switch (platform)
  {
  case Platform::INTEL:
    return "INTEL";
  case Platform::NVIDIA:
    return "NVIDIA CUDA";
  case Platform::NONE:
    return "";
  default:
    throw WrongPlatorfmException();
  }
}

DeviceSelector::Platform DeviceSelector::getPlatformFromUserString(std::string user)
{
  if (user == "intel") return Platform::INTEL;
  if (user == "nvidia") return Platform::NVIDIA;
  if (user == "") return Platform::NONE;
  throw WrongPlatorfmException();
}