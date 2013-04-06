#include <gtest\gtest.h>

#include <ScaleSpace.h>


TEST(InitScaleSpace, CheckNrScales)
{
  ScaleSpace ss;
  EXPECT_EQ(0, ss.getNrScales());
}

TEST(InitScaleSpace, CheckScaleStep)
{
  ScaleSpace ss;
  EXPECT_EQ(0, ss.getScaleStep());
}



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

TEST(SetScalesRange, SetStep1)
{
  ScaleSpace ss;
  EXPECT_THROW(ss.setScalesRange(2,1), ScaleSpaceException);
}

TEST(SetScalesRange, SetStep0)
{
  ScaleSpace ss;
  EXPECT_THROW(ss.setScalesRange(2,0), ScaleSpaceException);
}

TEST(SetScalesRange, SetMax0)
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



TEST(SetScaleStep, SetValidScaleStep)
{
  ScaleSpace ss;
  EXPECT_NO_THROW(ss.setScaleStep(2, 5));
  EXPECT_EQ(5, ss.getNrScales());
  EXPECT_EQ(2, ss.getScaleStep());
}

TEST(SetScaleStep, SetInvalidScaleStep)
{
  ScaleSpace ss;
  EXPECT_THROW(ss.setScaleStep(1, 5), ScaleSpaceException);
}

TEST(SetScaleStep, SetStep0)
{
  ScaleSpace ss;
  EXPECT_THROW(ss.setScaleStep(0, 2), ScaleSpaceException);
}

TEST(SetScaleStep, SetStepOdd)
{
  ScaleSpace ss;
  EXPECT_THROW(ss.setScaleStep(1, 2), ScaleSpaceException);
}

TEST(SetScaleStep, SetNr0)
{
  ScaleSpace ss;
  EXPECT_THROW(ss.setScaleStep(2, 0), ScaleSpaceException);
}

