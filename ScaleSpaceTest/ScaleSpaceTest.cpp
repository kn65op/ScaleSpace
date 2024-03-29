#include <gtest\gtest.h>

#include <ScaleSpaceOpenCL.h>

void test_no_change(ScaleSpaceOpenCL & ss)
{
  EXPECT_EQ(0, ss.getScaleStep());
  EXPECT_EQ(0, ss.getNrScales());
}


TEST(InitScaleSpaceOpenCL, CheckNrScales)
{
  ScaleSpaceOpenCL ss;
  EXPECT_EQ(0, ss.getNrScales());
}

TEST(InitScaleSpaceOpenCL, CheckScaleStep)
{
  ScaleSpaceOpenCL ss;
  EXPECT_EQ(0, ss.getScaleStep());
  EXPECT_EQ(0, ss.getNrScales());
}



TEST(SetScalesRange, SetValidRange1)
{
  ScaleSpaceOpenCL ss;
  EXPECT_TRUE(ss.setScalesRange(11,2));
  EXPECT_EQ(2, ss.getScaleStep());
  EXPECT_EQ(5, ss.getNrScales());
}

TEST(SetScalesRange, SetValidRange2)
{
  ScaleSpaceOpenCL ss;
  EXPECT_TRUE(ss.setScalesRange(9,4));
  EXPECT_EQ(4, ss.getScaleStep());
  EXPECT_EQ(2, ss.getNrScales());
}

TEST(SetScalesRange, SetStep1)
{
  ScaleSpaceOpenCL ss;
  EXPECT_THROW(ss.setScalesRange(2,1), ScaleSpaceException);
  test_no_change(ss);
}

TEST(SetScalesRange, SetStep0)
{
  ScaleSpaceOpenCL ss;
  EXPECT_THROW(ss.setScalesRange(2,0), ScaleSpaceZeroException);
  test_no_change(ss);
}

TEST(SetScalesRange, SetMax0)
{
  ScaleSpaceOpenCL ss;
  EXPECT_THROW(ss.setScalesRange(0,1), ScaleSpaceZeroException);
  test_no_change(ss);
}

TEST(SetScalesRange, SetValidRangeWithNoMaxInScalesSeletion)
{
  ScaleSpaceOpenCL ss;
  EXPECT_FALSE(ss.setScalesRange(20,6));
  EXPECT_EQ(6, ss.getScaleStep());
  EXPECT_EQ(3, ss.getNrScales());
}



TEST(SetScaleStep, SetValidScaleStep)
{
  ScaleSpaceOpenCL ss;
  EXPECT_NO_THROW(ss.setScaleStep(2, 5));
  EXPECT_EQ(5, ss.getNrScales());
  EXPECT_EQ(2, ss.getScaleStep());
}

TEST(SetScaleStep, SetInvalidScaleStep)
{
  ScaleSpaceOpenCL ss;
  EXPECT_THROW(ss.setScaleStep(1, 5), ScaleSpaceException);
  test_no_change(ss);
}

TEST(SetScaleStep, SetStep0)
{
  ScaleSpaceOpenCL ss;
  EXPECT_THROW(ss.setScaleStep(0, 2), ScaleSpaceZeroException);
  test_no_change(ss);
}

TEST(SetScaleStep, SetStepOdd)
{
  ScaleSpaceOpenCL ss;
  EXPECT_THROW(ss.setScaleStep(1, 2), ScaleSpaceException);
  test_no_change(ss);
}

TEST(SetScaleStep, SetNr0)
{
  ScaleSpaceOpenCL ss;
  EXPECT_THROW(ss.setScaleStep(2, 0), ScaleSpaceZeroException);
  test_no_change(ss);
}



TEST(SetMaxScale, SetValidMaxScale1)
{
  ScaleSpaceOpenCL ss;
  EXPECT_NO_THROW(ss.setMaxScale(5, 2));
  EXPECT_EQ(2, ss.getNrScales());
  EXPECT_EQ(2, ss.getScaleStep());
}

TEST(SetMaxScale, SetValidMaxScale2)
{
  ScaleSpaceOpenCL ss;
  EXPECT_NO_THROW(ss.setMaxScale(25, 4));
  EXPECT_EQ(4, ss.getNrScales());
  EXPECT_EQ(6, ss.getScaleStep());
}

TEST(SetMaxScale, SetInvalidMaxScale1)
{
  ScaleSpaceOpenCL ss;
  EXPECT_THROW(ss.setMaxScale(6, 2), ScaleSpaceException);
  test_no_change(ss);
}

TEST(SetMaxScale, SetInvalidMaxScale2)
{
  ScaleSpaceOpenCL ss;
  EXPECT_THROW(ss.setMaxScale(7, 2), ScaleSpaceException);
  test_no_change(ss);
}

TEST(SetMaxScale, SetMaxScale0)
{
  ScaleSpaceOpenCL ss;
  EXPECT_THROW(ss.setMaxScale(0, 2), ScaleSpaceZeroException);
  test_no_change(ss);
}

TEST(SetMaxScale, SetNr0)
{
  ScaleSpaceOpenCL ss;
  EXPECT_THROW(ss.setMaxScale(6, 0), ScaleSpaceZeroException);
  test_no_change(ss);
}
