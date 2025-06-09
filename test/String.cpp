#include "Utils/String.hpp"

#include "TestSetup.hpp"

TEST(SplitStringTest, ReturnValue)
{
  EXPECT_EQ(SplitString(std::string("12  34 56"), std::string(" "))[0], "12");
  EXPECT_EQ(SplitString(std::string("12  34 56"), std::string(" "))[1], "34");
  EXPECT_EQ(SplitString(std::string("  12 34 56"), std::string(" "))[0], "12");

  EXPECT_EQ(SplitString(std::string("12 34  56"), std::string(" "))[2], "56");
  EXPECT_EQ(SplitString(std::string("12 34 56  "), std::string(" "))[2], "56");

  EXPECT_EQ(SplitString(std::string("12 34 56"), std::string(" ")).size(), 3);
  EXPECT_EQ(SplitString(std::string("12 34 56  "), std::string(" ")).size(), 3);
  EXPECT_EQ(SplitString(std::string("  12 34 56  "), std::string(" ")).size(), 3);
}

TEST(StringStartsWithTest, ReturnValue)
{
  EXPECT_EQ(StringStartsWith(std::string{"prefix"}, std::string{"prefix"}), true);
  EXPECT_EQ(StringStartsWith(std::string{"prefix"}, std::string{"prefix "}), true);
  EXPECT_EQ(StringStartsWith(std::string{"prefix"}, std::string{" prefix"}), false);
  EXPECT_EQ(StringStartsWith(std::string{"prefix"}, std::string{" prefix "}), false);
}

TEST(SubstringCountTest, ReturnValue)
{
  EXPECT_EQ(SubstringCount(std::string{"subsubsub"}, std::string{"sub"}), 3);
  EXPECT_EQ(SubstringCount(std::string{"   subsubsub"}, std::string{"sub"}), 3);
  EXPECT_EQ(SubstringCount(std::string{"subsubsub   "}, std::string{"sub"}), 3);
  EXPECT_EQ(SubstringCount(std::string{"   subsubsub   "}, std::string{"sub"}), 3);
  EXPECT_EQ(SubstringCount(std::string{"   sub sub sub   "}, std::string{"sub"}), 3);
  EXPECT_EQ(SubstringCount(std::string{"   sub subsub   "}, std::string{"sub"}), 3);
  EXPECT_EQ(SubstringCount(std::string{""}, std::string{"sub"}), 0);
  EXPECT_EQ(SubstringCount(std::string{" "}, std::string{"sub"}), 0);
  EXPECT_EQ(SubstringCount(std::string{"   "}, std::string{"sub"}), 0);
  EXPECT_EQ(SubstringCount(std::string{"     "}, std::string{"sub"}), 0);
  EXPECT_EQ(SubstringCount(std::string{"su b"}, std::string{"sub"}), 0);
  EXPECT_EQ(SubstringCount(std::string{"s u b"}, std::string{"sub"}), 0);
  EXPECT_EQ(SubstringCount(std::string{"susus"}, std::string{"sus"}), 1);
  EXPECT_EQ(SubstringCount(std::string{"sususus"}, std::string{"sus"}), 2);
  EXPECT_EQ(SubstringCount(std::string{""}, std::string{""}), 0);
  EXPECT_EQ(SubstringCount(std::string{""}, std::string{"s"}), 0);
  EXPECT_EQ(SubstringCount(std::string{" "}, std::string{""}), 0);
  EXPECT_EQ(SubstringCount(std::string{"sss"}, std::string{"s"}), 3);
  EXPECT_EQ(SubstringCount(std::string{" sss"}, std::string{"s"}), 3);
  EXPECT_EQ(SubstringCount(std::string{"sss "}, std::string{"s"}), 3);
  EXPECT_EQ(SubstringCount(std::string{" sss "}, std::string{"s"}), 3);
}

TEST(RawStringArrayLengthTest, ReturnValue)
{
  const char **first = new const char*[4]{"123", "456", "78", ""};
  EXPECT_EQ(RawStringArrayLength(first, first + 3), 8);
}

TEST(ConcatRawStringArrayTest, ReturnValue)
{
  const char **first = new const char*[4]{"123", "456", "78", ""};
  EXPECT_EQ(ConcatRawStringArray(first, first + 4), std::string{"12345678"});
  EXPECT_EQ(ConcatRawStringArray(first, first + 4, std::string{","}), std::string{"123,456,78"});
}

TEST(ConcatRawStringArrayToRawTest, ReturnValue)
{
  const char **first = new const char*[4]{"123", "456", "78", ""};
  EXPECT_EQ(std::char_traits<char>::compare(ConcatRawStringArrayToRaw(first, first + 4), "12345678", 8), true);
  EXPECT_EQ(std::char_traits<char>::compare(ConcatRawStringArrayToRaw(first, first + 4, ","), "123,456,78", 10), true);

  EXPECT_EQ(std::char_traits<char>::compare(ConcatRawStringArrayToRaw({*first, *(first + 1), *(first + 2), *(first + 3)}), "12345678", 8), true);
  EXPECT_EQ(std::char_traits<char>::compare(ConcatRawStringArrayToRaw({*first, *(first + 1), *(first + 2), *(first + 3)}, ","), "123,456,78", 10), true);
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}