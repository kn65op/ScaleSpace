#pragma once

#include "getoptpp\getopt_pp.h"

#include "..\ScaleSpace\ScaleSpaceOpenCL.h"
#include <string>

/**
 * @class Class controls parameters and applicatins flow.
 */
class ProgramController
{
public:
  /**
   * Constructor with command line arguments.
   * @param argc Number of arguments.
   * @param argv Arguments.
   */
  ProgramController(int argc, char * argv[]);
  ~ProgramController(void);
  
  void printHelp() const;
  bool areOptionsValid() const;
  ScaleSpaceMode getMode() const;
  ScaleSpaceSourceImageType getSourceImageType() const;
  ScaleSpaceProcessor getProcessor() const;
  std::string getInputFile() const;
  std::string getInputPrefix() const;
  std::string getOutputPrefix() const;
  unsigned int getNrScales() const
  {
    return nr_scales;
  }

  unsigned int getScaleStep() const
  {
    return scale_step;
  }

  bool useCamera() const;
  bool isDeviceInfo() const;

  void printProgramInfo() const;

  bool oneInputFile() const;
  bool isShow() const;
  bool isQuiet() const;
  bool isDebug() const;
  bool calcFirstImage() const;
  std::string getOpenCL() const;

private:
  ProgramController(const ProgramController & ); //non copyable
  
  void processArgs(int argc, char * argv[]);
  
  void getModeFromOptions();
  void getScalesFromOptions();
  void getTypeFromOptions();
  void getProcessorFromOptions();
  void getInputFromOptions();
  void getOutputPrefixFromOptions();
  void getInputPrefixFromOptions();
  void getOpenCLFromOptions();

  GetOpt::GetOpt_pp opt;
  
  bool help;
  bool device_info;
  bool show;
  bool quiet;
  bool debug;
  bool calc_first_image;

  std::string error_message;
  //parameters
  ScaleSpaceMode mode;
  ScaleSpaceSourceImageType type;
  ScaleSpaceProcessor processor;
  std::string in_file;
  std::string out_prefix;
  std::string in_prefix;
  std::string opencl;

  unsigned int scale_step;
  unsigned int nr_scales;
  
};

