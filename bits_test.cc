#include "puz/bits.h"

#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace puzzle {
TEST(BitOps, simple) {
  EXPECT_EQ(SetBit(0, 0), 1);
  EXPECT_EQ(SetBit(0, 1), 2);
  EXPECT_EQ(SetBit(0, 2), 4);
  EXPECT_EQ(SetBit(7, 2), 7);
  EXPECT_EQ(SetBit(9, 2), 13);

  EXPECT_EQ(UnsetBit(15, 0), 14);
  EXPECT_EQ(UnsetBit(15, 1), 13);
  EXPECT_EQ(UnsetBit(15, 2), 11);
  EXPECT_EQ(UnsetBit(15, 3), 7);
  EXPECT_EQ(UnsetBit(0, 3), 0);
  EXPECT_EQ(UnsetBit(9, 1), 9);
  EXPECT_EQ(UnsetBit(9, 3), 1);

  EXPECT_EQ(TestBit(0, 0), false);
  EXPECT_EQ(TestBit(9, 0), true);
  EXPECT_EQ(TestBit(9, 1), false);
  EXPECT_EQ(TestBit(9, 2), false);
  EXPECT_EQ(TestBit(9, 3), true);
}

TEST(BitIterator, simple) {
  BitIterator b(65535);
  std::vector<int> result;
  while (b.HasNext()) {
    result.push_back(b.Next());
  }
  std::vector<int> expected(
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15});
  EXPECT_THAT(result, ::testing::ElementsAreArray(expected));
}

TEST(BitIterator, simple2) {
  BitIterator b(257);
  std::vector<int> result;
  while (b.HasNext()) {
    result.push_back(b.Next());
  }
  std::vector<int> expected({0, 8});
  EXPECT_THAT(result, ::testing::ElementsAreArray(expected));
}

TEST(Project2, simple) {
  EXPECT_THAT(Project2(255, std::vector<int>{0, 1, 5}), 7);
  EXPECT_THAT(Project2(6, std::vector<int>{0, 1, 2}), 6);
  EXPECT_THAT(Project2(27, std::vector<int>{0, 1, 2}), 3);
  EXPECT_THAT(Project2(27, std::vector<int>{4, 2, 3}), 5);
  EXPECT_THAT(Project2(4, std::vector<int>{0}), 0);
}

TEST(Project, simple) {
  // Empty set.
  uint16_t a = 0b0000000000000000;
  // Therefore, projection is also empty.
  EXPECT_THAT(Project(a, std::vector<int>{0}), 0b0000);

  // Only the last bit is set.
  a = 0b0000000000000010;  // 0000:0, 0001:1, xxxx:0
  // Projection of other bits is always 0, which is represented by 01 ==> 1.
  EXPECT_THAT(Project(a, std::vector<int>{1}), 0b0001);
  EXPECT_THAT(Project(a, std::vector<int>{2}), 0b0001);
  EXPECT_THAT(Project(a, std::vector<int>{1, 2}), 0b0001);
  // Projection of 0th bit is always 1, which is represented by 10 ==> 2.
  EXPECT_THAT(Project(a, std::vector<int>{0}), 0b0010);
  // Projection of 0th and another bit is again always 1.
  EXPECT_THAT(Project(a, std::vector<int>{0, 3}), 0b0010);

  // All possibilities are there.
  a = 0b1111111111111111;
  // Projection of a single bit is always 3. (It may or may not bet set).
  EXPECT_THAT(Project(a, std::vector<int>{0}), 0b11);
  EXPECT_THAT(Project(a, std::vector<int>{1}), 0b11);
  EXPECT_THAT(Project(a, std::vector<int>{2}), 0b11);
  EXPECT_THAT(Project(a, std::vector<int>{3}), 0b11);
  // Projection of a couple bits is 15. (00, 01, 10, 11 ==> 1111 => 15)
  EXPECT_THAT(Project(a, std::vector<int>{1, 3}), 0b1111);

  // First bit is always set. second bit is sometimes set.
  a = 10;  // 0000 => false, 0001 => true, 0010 => false, 0011 => true.
  // Projection of the 0th bit is 1 : which is represented by 10 ==> 2.
  EXPECT_THAT(Project(a, std::vector<int>{0}), 0b10);
  // Projection of the 1st bit is 3 : it may or may not be set.
  EXPECT_THAT(Project(a, std::vector<int>{1}), 3);
  // Projection of the 2nd bit is always 0, which is represented by 01 => 1.
  EXPECT_THAT(Project(a, std::vector<int>{2}), 1);

  // Possibilities are 0001 (2), 0011 (8), 0101 (32)
  a = 42;
  // Projection of 0th and 2nd bit is 11 -- 01 -- ==> 1010 ==> 10.
  EXPECT_THAT(Project(a, std::vector<int>{0, 2}), 10);
}

TEST(Reduce, simple) {
  // m is 1 ==> 11:F 10:F 01: F 00:T
  // which means both bits should be 0.
  uint16_t m = 0b0001;
  // We look at the 1st and 3rd bits, so m corresponds to 0x0x mask.
  std::vector<int> indexes = {1, 3};
  // n : 0000, 0011, 0101, 0110
  uint16_t n = (1 << 0) + (1 << 3) + (1 << 5) + (1 << 6);
  // Only the ones that fit into 0x0x should remain.
  EXPECT_THAT(Reduce(n, m, indexes), (1 << 0) + (1 << 5));

  // m is 6 ==> 11:F 10:T 01:T 00:F
  // which means XOR of the bits should be 1.
  m = 0b0110;
  // We look at the 0th and 3rd bits, so m corresponds to AxxA' mask.
  indexes = {0, 3};
  // n : 0001, 0010, 1000, 1001
  n = (1 << 1) + (1 << 2) + (1 << 8) + (1 << 9);
  // Only the ones that fit into AxxA' should remain.
  EXPECT_THAT(Reduce(n, m, indexes), (1 << 1) + (1 << 8));
}

TEST(ReduceSingle, simple) {
  uint16_t n = (1 << 0) + (1 << 2) + (1 << 3);
  EXPECT_THAT(ReduceSingle(n, 1, true), (1 << 2) + (1 << 3));
  EXPECT_THAT(ReduceSingle(n, 1, false), (1 << 0));
}

TEST(ProjectSingle, simple) {
  uint16_t n = (1 << 2) + (1 << 6);
  EXPECT_THAT(ProjectSingle(n, 0), 1);
  EXPECT_THAT(ProjectSingle(n, 1), 2);
  EXPECT_THAT(ProjectSingle(n, 2), 3);
}

}  // namespace puzzle
