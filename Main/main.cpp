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
    OpenCLImageAlgorithm *gaussian = new OpenCLGaussianImage();

    cv::Mat gaussian_kernel = cv::getGaussianKernel(400, -1, CV_32F);

    OpenCLGaussianParams params;
    params.setMask(20, gaussian_kernel.data);
    gaussian->setParams(params);

    gaussian_stream.pushAlgorithm(gaussian);
    
    OpenCLDevice device = OpenCLDevice::getDevices().front();

    cv::Mat input, input_tmp;
    input_tmp = cv::imread("in.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    input_tmp.convertTo(input, CV_32F, 1.0/255.0f);
    
    cv::Mat output(input.size(), input.type());
    
    gaussian_stream.setDataSize(input.size().width, input.size().height);
    
    gaussian_stream.setDevice(device);

    gaussian_stream.prepare();

    gaussian_stream.processImage(input.data, output.data);

    std::cout << output.at<float>(0,0) << "\n";
    std::cout << gaussian_stream.getTime() << "\n";
    output.convertTo(output, CV_8U, 255.0);
    cv::imwrite("out.bmp", output);
  }
  catch (OpenCLException ex)
  {
    std::cout << ex.getFullMessage() << "\n";
  }

  return a;
}