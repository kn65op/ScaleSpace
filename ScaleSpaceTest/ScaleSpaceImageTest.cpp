#include <gtest\gtest.h>

#include <ScaleSpaceImage.h>

TEST(GetDataForScale, NonScaleNonReturn)
{
  ScaleSpaceImage ssi;
  EXPECT_THROW(ssi.getDataForScale(23), ScaleSpaceImageException);
}

TEST(GetDataForScale, Return)
{
  ScaleSpaceImage ssi;
  ssi.createImage(1,1, 10, CV_32FC1);
  void * ret;
  ASSERT_NO_THROW(ret = ssi.getDataForScale(10));
  EXPECT_NE(nullptr, ret);
}