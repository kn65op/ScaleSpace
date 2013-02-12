#include <OpenCLGaussian.h>
#include <OpenCLDevice.h>

#include <opencv\cv.h>
#include <opencv\highgui.h>

#include <iostream>

int main()
{
  int a = 0;
  try
  {
    OpenCLGaussian gaussian;
    
    OpenCLDevice device = OpenCLDevice::getDevices().front();
    
    gaussian.setDevice(device);

    gaussian.prepare();

    cv::Mat input, input_tmp;
    input_tmp = cv::imread("in.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    input_tmp.convertTo(input, CV_32F, 1.0/255.0f);
    
    cv::Mat output_tmp(input.rows - 2, input.cols - 2, input.type());
    
    gaussian.run(input.data, input.total() * input.elemSize(), output_tmp.data, output_tmp.total() * output_tmp.elemSize());

    cv::Mat output;

    output_tmp.convertTo(output, CV_8U, 255);
    cv::imwrite("out.bmp", output);
  }
  catch (OpenCLException ex)
  {
    std::cout << ex.getFullMessage() << "\n";
  }

  return a;
}