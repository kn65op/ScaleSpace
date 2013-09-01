#include "ProgramController.h"

#include <iostream>
#include <vector>

ProgramController::ProgramController(int argc, char* argv []) :
  type(ScaleSpaceSourceImageType::Gray),
  mode(ScaleSpaceMode::Pure),
  in_file(""),
  help(false),
  opt(GetOpt::GetOpt_pp(argc, argv)),
  scale_step(10),
  nr_scales(10)
{
  processArgs(argc, argv);
}


ProgramController::~ProgramController(void)
{
}

void ProgramController::processArgs(int argc, char*argv[])
{
  if (opt >> GetOpt::OptionPresent('h', "help"))
  {
    help = true;
    return;
  }
  opt >> GetOpt::Option('i', "in", in_file);
  getModeFromOptions();
  getScalesFromOptions();
  getTypeFromOptions();
  getProcessorFromOptions();
  device_info = opt >> GetOpt::OptionPresent("device_info");
  if (opt.options_remain())
  {
    error_message = "Unexpected options";
    help = true;
  }
}

void ProgramController::printHelp() const
{
  if (error_message != "")
  {
    std::cout << "Error: \n";
    std::cout << error_message << "\n";
  }
  std::cout << "Usage:\n";
  std::cout << "ScaleSpace [options]\n";
  std::cout << "If options is not provided as input will be used JAI camera and 10 scales with step 10. If mode is not provided it will calculate only Scale Space representation.\n";

  std::cout << "Options:\n";
  std::cout << "\t-i,--in\t\tinput file instead of use camera. Default input file type is gray.\n";
  std::cout << "\t-m,--mode\tScale Space mode. One of: blob, corner, edge, ridge.\n";
  std::cout << "\t-t,--type\tInput file type. Can be bayer or gray. It works only with file.\n";
  std::cout << "\t-p,--processor\tSelect implementation. It can be:\n";
  std::cout << "\t\t\t\tcl, opencl - for OpenCL implementation\n";
  std::cout << "\t\t\t\tcv, opencv, cpu - for OpenCV implementation\n";
  std::cout << "\t\t\t\tcv_gpu, opencv_gpu, gpu - for OpenCV GPU implementation\n";
  std::cout << "\t-s,--scale\tSet scales in format a b, where a is scale step and b is number of scales\n";
  //std::cout << "\t-,--\t\n";
  //std::cout << "\t-,--\t\n";
}

bool ProgramController::areOptionsValid() const
{
  return !help;
}

void ProgramController::getModeFromOptions()
{
  std::string mode_s;
  if (opt >> GetOpt::Option('m', "mode", mode_s))
  {
    if (mode_s == "laplacian")
    {
      mode = ScaleSpaceMode::Laplacian;
    }
    else if (mode_s == "edge")
    {
      mode = ScaleSpaceMode::Edges;
    }
    else if (mode_s == "blob")
    {
      mode = ScaleSpaceMode::Blobs;
    }
    else if (mode_s == "corner")
    {
      mode = ScaleSpaceMode::Corners;
    }
    else if (mode_s == "ridge")
    {
      mode = ScaleSpaceMode::Ridges;
    }
    else
    {
      help = true;
      error_message = "For mode supported values are: blob, edge, blob2, corner, ridge.";
    }
  }   
}

void ProgramController::getTypeFromOptions()
{
  std::string type_s;
  if (opt >> GetOpt::Option('t', "type", type_s))
  {
    if (type_s == "gray")
    {
      type = ScaleSpaceSourceImageType::Gray;
    }
    else if (type_s == "bayer")
    {
      type = ScaleSpaceSourceImageType::Bayer;
    }
    else
    {
      help = true;
      error_message = "For type supported values are: gray, bayer.";
    }
  }   
  else
  {
    if (in_file == "")
    {
      type = ScaleSpaceSourceImageType::Bayer;
    }
  }
}

void ProgramController::getProcessorFromOptions()
{
  std::string type_p;
  if (opt >> GetOpt::Option('p', "processor", type_p))
  {
    if (type_p == "cl" || type_p == "opencl")
    {
      processor = ScaleSpaceProcessor::OPENCL;
    }
    else if (type_p == "cpu" || type_p == "opencv" || type_p == "cv")
    {
      processor = ScaleSpaceProcessor::OPENCV_CPU;
    }
    else if (type_p == "gpu" || type_p == "opencv_gpu" || type_p == "cv_gpu")
    {
      processor = ScaleSpaceProcessor::OPENCV_GPU;
    }
    else
    {
      help = true;
      error_message = "For processor supported values are: opencv (cv), opencl (cpu, cl), opencl_gpu (cl_gpu, gpu)";
    }
  }
  else
  {
    processor = ScaleSpaceProcessor::OPENCL;
  }
}


ScaleSpaceMode ProgramController::getMode() const
{
  return mode;
}

std::string ProgramController::getInputFile() const
{
  return in_file;
}

void ProgramController::getScalesFromOptions()
{
  std::vector<unsigned int> scale;
  if (opt >> GetOpt::Option('s', "scale", scale))
  {
    if (scale.size() != 2)
    {
      help = true;
      error_message = "Scale is: a b, where a is scale step and b is number of scales";
      return;
    }
    scale_step = scale[0];
    nr_scales = scale[1];
  }
}

bool ProgramController::useCamera() const
{
  return in_file == "";
}

ScaleSpaceSourceImageType ProgramController::getSourceImageType() const
{
  return type;
}

ScaleSpaceProcessor ProgramController::getProcessor() const
{
  return processor;
}

bool ProgramController::isDeviceInfo() const
{
  return device_info;
}

void ProgramController::printProgramInfo() const
{
  std::cout << "Program parameters:\n";
  std::cout << "\tProcessor: " << processor << "\n";
  std::cout << "\tMode: " << mode << "\n";
  std::cout << "\tInput: ";
  if (in_file == "")
  {
    std::cout << "camera";
  }
  else
  {
    std::cout << in_file;
  }
  std::cout << "\n";
  std::cout << "\tInput image type: " << type << "\n";
  std::cout << "\tScales: " << nr_scales << "\n\tStep: " << scale_step << "\n";
}
