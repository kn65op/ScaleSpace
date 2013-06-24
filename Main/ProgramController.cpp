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
}

void ProgramController::printHelp() const
{
  std::cout << "Help\n";
  if (error_message != "")
  {
    std::cout << error_message << "\n";
  }
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
    if (mode_s == "blob")
    {
      mode = ScaleSpaceMode::Laplacian;
    }
    else if (mode_s == "edge")
    {
      mode = ScaleSpaceMode::Edges;
    }
    else if (mode_s == "blob2")
    {
      mode = ScaleSpaceMode::Blobs;
    }
    else if (mode_s == "corner")
    {
      mode = ScaleSpaceMode::Corners;
    }
    else
    {
      help = true;
      error_message = "For mode supported values are: blob, edge, blob2, corner.";
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