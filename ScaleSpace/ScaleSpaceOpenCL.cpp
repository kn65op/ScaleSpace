#include "ScaleSpaceOpenCL.h"
#include "DeviceSelector.h"

#include <opencv\cv.h>

#include <OpenCLGaussian.h>
#include <OpenCLIntToFloat.h>
#include <OpenCLFloatToInt.h>
#include <OpenCLLaplacian.h>
#include <OpenCLEdgeDetector.h>
#include <OpenCLRidgeDetector.h>
#include <OpenCLCornerDetector.h>
#include <OpenCLBlobDetector.h>
#include <OpenCLFindMaxin3DImage.h>
#include <OpenCLFindEdgesIn3DImage.h>
#include <OpenCLFindRidgesIn3DImage.h>
#include <OpenCLFindMaxin2DImage.h>
#include <OpenCLFindEdgesIn2DImage.h>
#include <OpenCLFindRidgesIn2DImage.h>
#include <OpenCLBayerFilter.h>
#include <OpenCLRGBToGray.h>

#include <Stoper.h>

using namespace TTime;

ScaleSpaceOpenCL::ScaleSpaceOpenCL(ScaleSpaceMode mode /* = Pure */) :
  ScaleSpace(mode)
{
  if (OpenCLDevice::getDevices().empty())
  {
    throw NoPlatformScaleSpaceException("OpenCL platform not found on system");
  }
  nr_scales = 0;
  scale_step = 0;
  streams =  streams_t();
  prepared = false;

  last_height = last_width = last_scale = 0;

  post_processing = nullptr;
}

ScaleSpaceOpenCL::~ScaleSpaceOpenCL(void)
{
  clearStreams();
}


void ScaleSpaceOpenCL::clearStreams()
{
  for (auto s : streams)
  {
    delete s;
  }
  streams.clear();
}

void ScaleSpaceOpenCL::prepare(ScaleSpaceSourceImageType si_type, ScaleSpaceOutputType out_type)
{
  output_type = out_type;
  if (!streams.empty())
  {
    clearStreams();
    //throw ScaleSpaceZeroException("prepare: There is no scales to prepare");
  }

  DeviceSelector ds;
  OpenCLDevice device = ds.selectDevice(parameters.device);

  for (unsigned int i=0; i< nr_scales; ++i)
  {
    auto s = new OpenCLAlgorithmsStream();

    OpenCL2DTo2DImageAlgorithmForStream *itf = new OpenCLIntToFloat();
    s->pushAlgorithm(itf);

    if (si_type == ScaleSpaceSourceImageType::Bayer)
    {
      OpenCL2DTo2DImageAlgorithmForStream *bayer = new OpenCLBayerFilterForStream();
      OpenCLBayerFilterParams bayer_params;
      bayer->setParams(bayer_params);
      s->pushAlgorithm(bayer);

      OpenCL2DTo2DImageAlgorithmForStream *rgb2gray = new OpenCLRGBToGray();
      s->pushAlgorithm(rgb2gray);
    }

    unsigned int scale = getGaussianSizeForScale(i);
    OpenCL2DTo2DImageAlgorithmForStream *gaussian = new OpenCLGaussianImage(scale/2);

    cv::Mat gaussian_kernel_2d = getGaussianForScale(i);

    OpenCLGaussianParams params;
    params.setMask(scale, gaussian_kernel_2d.data);
    gaussian->setParams(params);
    s->pushAlgorithm(gaussian);

    OpenCL2DTo2DImageAlgorithmForStream *recognizer = nullptr;
    OpenCLLaplacianParams laplacian_params;
    laplacian_params.setSigma(sigmas[i]*sigmas[i]);
    switch (calc_mode)
    {
    case ScaleSpaceMode::Laplacian:
      recognizer = new OpenCLLaplacian();
      recognizer->setParams(laplacian_params);
      s->pushAlgorithm(recognizer);
      type = CV_32FC1;
      switch (out_type)
      {
      case ScaleSpaceOutputType::ONE_IMAGE:
        post_processing = new OpenCLFindMaxin3DImage();
        break;
      case ScaleSpaceOutputType::IMAGE_FOR_SCALE:
        post_processing = new OpenCLFindMaxin2DImage();
        break;
      }
      break;
    case ScaleSpaceMode::Edges:
      recognizer = new OpenCLEdgeDetector();
      s->pushAlgorithm(recognizer);
      type = CV_32FC1;
      switch (out_type)
      {
      case ScaleSpaceOutputType::ONE_IMAGE:
        post_processing = new OpenCLFindEdgesIn3DImage();
        break;
      case ScaleSpaceOutputType::IMAGE_FOR_SCALE:
        post_processing = new OpenCLFindEdgesIn2DImage();
        break;
      }
      break;
    case ScaleSpaceMode::Blobs:
      recognizer = new OpenCLBlobDetector();
      s->pushAlgorithm(recognizer);
      type = CV_32FC1;
      switch (out_type)
      {
      case ScaleSpaceOutputType::ONE_IMAGE:
        post_processing = new OpenCLFindMaxin3DImage();
        break;
      case ScaleSpaceOutputType::IMAGE_FOR_SCALE:
        post_processing = new OpenCLFindMaxin2DImage();
        break;
      }
      break;
    case ScaleSpaceMode::Corners:
      recognizer = new OpenCLCornerDetector();
      s->pushAlgorithm(recognizer);
      type = CV_32FC1;
      switch (out_type)
      {
      case ScaleSpaceOutputType::ONE_IMAGE:
        post_processing = new OpenCLFindMaxin3DImage();
        break;
      case ScaleSpaceOutputType::IMAGE_FOR_SCALE:
        post_processing = new OpenCLFindMaxin2DImage();
        break;
      }
      break;
    case ScaleSpaceMode::Ridges:
      recognizer = new OpenCLRidgeDetector();
      s->pushAlgorithm(recognizer);
      type = CV_32FC1;
      switch (out_type)
      {
      case ScaleSpaceOutputType::ONE_IMAGE:
        post_processing = new OpenCLFindRidgesIn3DImage();
        break;
      case ScaleSpaceOutputType::IMAGE_FOR_SCALE:
        post_processing = new OpenCLFindRidgesIn2DImage();
        break;
      }
      break;
    default: //and Pure
      //throw ScaleSpaceException("Pure not working for now");
      //recognizer = new OpenCLFloatToInt();
      //s->pushAlgorithm(recognizer);
      type = CV_32FC1;
      break;
    }

    if(post_processing)
    {
      post_processing->setDevice(device);
    }
    s->setDevice(device);

    streams.push_back(s);
  }

  prepared = true;
}

void ScaleSpaceOpenCL::process(ScaleSpaceImage & image, bool first_image)
{
  if (!prepared)
  {
    std::cout << "Algorithm preparation using grayscale input";
    prepare(ScaleSpaceSourceImageType::Gray);
  }

  if (last_height != image.size().height || last_width != image.size().width || last_scale != nr_scales)
  {
    unsigned int images = 1;
    switch (calc_mode)
    {
    case ScaleSpaceMode::Edges:
      images = 2;
      break;
    case ScaleSpaceMode::Ridges:
      images = 2;
      break;
    }
    image.createImage(nr_scales, type, images);
    for (auto s : streams)
    {
      s->setDataSize(image.size().width, image.size().height); //not like OpenCV
      s->prepare();
    }
    last_height = image.size().height;
    last_width = image.size().width;
    last_scale = nr_scales;

    if (post_processing)
    {
      post_processing->setDataSize(image.size().width, image.size().height, nr_scales);
      post_processing->prepare();
      if (output_type == ScaleSpaceOutputType::ONE_IMAGE)
      {
        if (calc_mode == ScaleSpaceMode::Edges)  //TODO: fix 
        {
          OpenCLFindEdgesIn3DImageParams edges_params;
          edges_params.setData(image.getDataForScale(0,1));
          post_processing->setParams(edges_params);
        }
        if (calc_mode == ScaleSpaceMode::Ridges) //TODO: fix
        {
          OpenCLFindRidgesIn3DImageParams ridge_params;
          ridge_params.setData(image.getDataForScale(0,1));
          post_processing->setParams(ridge_params);
        }
      }
    }
  }

  int i = 0;
  for (auto s : streams)
  {
    if (!first_image || parameters.calc_first_image)
    {
      Stoper::start("gaussian" + std::to_string(i), false);
      Stoper::start("gaussian", false);
    }
    s->processImage(image.getInputData(), image.getDataForScale(i));
    if (!first_image || parameters.calc_first_image)
    {
      Stoper::stop("gaussian");
      Stoper::stop("gaussian" + std::to_string(i));
    }
    void * additional_output = s->getLastAlgorithmAdditionalOutput();
    if (additional_output)
    {
      memcpy(image.getDataForScale(i, 1), additional_output, image.getOneImageSize());
    }
    ++i;
  }
  //image.show("before"); 
  cv::Mat outp = cv::Mat::zeros(image.size(), image.type());
  if (post_processing)
  {
    if (output_type == ScaleSpaceOutputType::ONE_IMAGE)
    {
      post_processing->processData(image.getDataForScale(0), outp.data);
    }
    else if (output_type == ScaleSpaceOutputType::IMAGE_FOR_SCALE)
    {
      for (unsigned int i = 0; i < nr_scales ; ++i)
      {
        if (calc_mode == ScaleSpaceMode::Edges)
        {
          OpenCLFindEdgesIn2DImageParams params;
          params.setData(image.getDataForScale(i,1));
          post_processing->setParams(params);
        }
        if (calc_mode == ScaleSpaceMode::Ridges)
        {
          OpenCLFindRidgesIn2DImageParams params;
          params.setData(image.getDataForScale(i,1));
          post_processing->setParams(params);
        }

        post_processing->processData(image.getDataForScale(i), image.getDataForOutput(i));
      }
    }
  }
  switch (calc_mode)
  {
  case ScaleSpaceMode::Laplacian:
    // output.show(outp, sigmas); 
    outp = outp * 255 / nr_scales;
    break;
  case ScaleSpaceMode::Edges:
    break;
  case ScaleSpaceMode::Blobs:
    //  output.show(outp, sigmas); 
    outp = outp * 255 / nr_scales;
    break;
  case ScaleSpaceMode::Corners:
    outp = outp * 255 / nr_scales;
    break;
  case ScaleSpaceMode::Ridges:
    break;
  default: //and Pure
    break;
  }
}
