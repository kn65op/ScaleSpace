#include <OpenCLGaussian.h>
#include <OpenCLAlgorithmsStream.h>

#include <opencv\cv.h>
#include <opencv\highgui.h>

#include <iostream>

int main()
{
  int a = 0;
  try
  {
    OpenCLAlgorithmsStream gaussian_stream;
    OpenCLAlgorithm *gaussian = new OpenCLGaussianImage();

    gaussian_stream.pushAlgorithm(gaussian);
    
    OpenCLDevice device = OpenCLDevice::getDevices().front();
    
    gaussian_stream.setDevice(device);

    gaussian_stream.prepare();

    cv::Mat input, input_tmp;
    input_tmp = cv::imread("in.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    input_tmp.convertTo(input, CV_32F, 1.0/255.0f);
    
    cv::Mat output(input.size(), input.type());
    
    gaussian_stream.setDataSize(input.size().width, input.size().height));

    gaussian_stream.processImage(input.data, output.data);

    output.convertTo(output, CV_8U, 255);
    cv::imwrite("out.bmp", output);
  }
  catch (OpenCLException ex)
  {
    std::cout << ex.getFullMessage() << "\n";
  }

  return a;
}