#pragma once

#include <ScaleSpaceFactory.h>
#include <Stoper.h>

#include "ProgramController.h"

void printCLDeviceInfo();

void processScaleSpace(ScaleSpace *ss, ProgramController & controller, cv::Mat & input, ScaleSpaceImage & output, TTime::Stoper & stoper, bool show);

std::string getFileWithPrefix(std::string prefix, unsigned int i);