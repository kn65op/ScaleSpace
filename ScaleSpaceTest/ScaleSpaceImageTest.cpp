#include <gtest\gtest.h>

#include <ScaleSpaceImage.h>

TEST(GetDataForScale, NonScaleNonReturn)
{
  ScaleSpaceImage ssi;
  EXPECT_THROW(ssi.getDataForScale(23), ScaleSpaceImageException);
}
