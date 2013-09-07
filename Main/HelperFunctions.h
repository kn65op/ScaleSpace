#pragma once

#include <ScaleSpaceFactory.h>
#include <Stoper.h>

#include "ProgramController.h"

void printCLDeviceInfo();

void processScaleSpace(ScaleSpace *ss, ProgramController & controller, cv::Mat & input, ScaleSpaceImage & output, TTime::Stoper & stoper, bool show, unsigned int image_number, bool calc_first_image);

std::string getFileWithPrefix(std::string prefix, unsigned int i);