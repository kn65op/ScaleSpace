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
  unsigned int getNrScales() const
  {
    return nr_scales;
  }

  unsigned int getScaleStep() const
  {
    return scale_step;
  }

  bool useCamera() const;
  
private:
  ProgramController(const ProgramController & ); //non copyable
  
  void processArgs(int argc, char * argv[]);
  
  void getModeFromOptions();
  void getScalesFromOptions();
  void getTypeFromOptions();
  void getProcessorFromOptions();

  GetOpt::GetOpt_pp opt;
  
  bool help;
  std::string error_message;
  //parameters
  ScaleSpaceMode mode;
  ScaleSpaceSourceImageType type;
  ScaleSpaceProcessor processor;
  std::string in_file;

  unsigned int scale_step;
  unsigned int nr_scales;
  
};

