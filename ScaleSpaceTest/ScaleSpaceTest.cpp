#include <gtest\gtest.h>

#include <ScaleSpace.h>


TEST(AssertTrue, AssertTrue)
{
  ScaleSpace ss;
  ScaleSpaceImage sci;
  ASSERT_TRUE(true);
}

int main(int argc, char** argv) 
{ 
    testing::InitGoogleTest(&argc, argv); 
    RUN_ALL_TESTS(); 
}
