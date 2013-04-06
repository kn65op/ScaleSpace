#include <gtest\gtest.h>

#include <ScaleSpace.h>


TEST(SetScalesRange, SetValidRange1)
{
  ScaleSpace ss;
  EXPECT_TRUE(ss.setScalesRange(11,2));
  EXPECT_EQ(2, ss.getScaleStep());
  EXPECT_EQ(5, ss.getNrScales());
}

TEST(SetScalesRange, SetValidRange2)
{
  ScaleSpace ss;
  EXPECT_TRUE(ss.setScalesRange(11,4));
  EXPECT_EQ(4, ss.getScaleStep());
  EXPECT_EQ(2, ss.getNrScales());
}

TEST(SetScalesRange, SetInvalidStep)
{
  ScaleSpace ss;
  EXPECT_THROW(ss.setScalesRange(2,1), ScaleSpaceException);
}

TEST(SetScalesRange, SetInvalidMax)
{
  ScaleSpace ss;
  EXPECT_THROW(ss.setScalesRange(0,1), ScaleSpaceException);
}

TEST(SetScalesRange, SetValidRangeWithNoMaxInScalesSeletion)
{
  ScaleSpace ss;
  EXPECT_FALSE(ss.setScalesRange(20,6));
  EXPECT_EQ(6, ss.getScaleStep());
  EXPECT_EQ(3, ss.getNrScales());
}