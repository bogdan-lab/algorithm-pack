#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "algorithm_pack/treap.h"

using ::testing::IsEmpty;
using ::testing::IsNull;
using ::testing::Not;
using ::testing::NotNull;

TEST(TreapTest, CreateEmpty) {
  alpa::Treap<int, std::string> test;
  EXPECT_TRUE(test.Empty());
  EXPECT_EQ(test.Size(), 0);
}

TEST(TreapTest, CreateAndFill) {
  constexpr std::array<int, 11> kInput{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  alpa::Treap<int, int> test(/*seed=*/kInput.size());
  for (size_t i = 0; i < kInput.size(); ++i) {
    int input_val = kInput[i] * kInput[i];
    int* val = test.Insert(kInput[i], input_val);
    ASSERT_THAT(val, NotNull());
    EXPECT_EQ(*val, input_val);
    EXPECT_EQ(test.Size(), i + 1);
  }
}

TEST(TreapTest, FindInEmpty) {
  alpa::Treap<int, std::string> test;
  std::string* res = test.Find(1);
  EXPECT_THAT(res, IsNull());
}

TEST(TreapTest, FindExisting) {
  constexpr std::array<uint32_t, 7> kInput{0, 1, 2, 3, 4, 5, 6};
  auto input = kInput;
  std::vector<std::string> vals(input.size());
  std::transform(input.begin(), input.end(), vals.begin(),
                 [](uint32_t val) { return std::to_string(val); });
  do {
    std::next_permutation(vals.begin(), vals.end());
    alpa::Treap<uint32_t, std::string> test(/*seed=*/input.front());
    for (size_t i = 0; i < input.size(); ++i) {
      std::string* res = test.Insert(input[i], vals[i]);
      ASSERT_THAT(res, NotNull());
      EXPECT_EQ(*res, vals[i]);
    }
    for (size_t i = 0; i < input.size(); ++i) {
      std::string* res = test.Find(input[i]);
      ASSERT_THAT(res, NotNull());
      EXPECT_EQ(*res, vals[i]);
    }
  } while (std::next_permutation(input.begin(), input.end()));
}

TEST(TreapTest, FindNotExisting) {
  constexpr std::array<uint32_t, 7> kInput{2, 4, 6, 8, 10, 12, 14};
  constexpr std::array<uint32_t, 8> kChecks{1, 3, 5, 7, 9, 11, 13, 15};
  auto input = kInput;
  std::vector<std::string> vals{"a", "b", "c", "d", "e", "f", "g"};
  do {
    std::next_permutation(vals.begin(), vals.end());
    alpa::Treap<uint32_t, std::string> test(/*seed=*/input.front());
    for (size_t i = 0; i < input.size(); ++i) {
      std::string* res = test.Insert(input[i], vals[i]);
      ASSERT_THAT(res, NotNull());
      EXPECT_EQ(*res, vals[i]);
    }
    for (const auto& key : kChecks) {
      EXPECT_THAT(test.Find(key), IsNull());
    }
  } while (std::next_permutation(input.begin(), input.end()));
}

TEST(TreapTest, InsertAndModifyNewValue) {
  constexpr std::array<uint32_t, 7> kInput{0, 2, 4, 6, 8, 10, 12};
  auto input = kInput;
  std::vector<std::string> vals{"a", "b", "c", "d", "e", "f", "g"};
  do {
    std::next_permutation(vals.begin(), vals.end());
    alpa::Treap<uint32_t, std::string> test(/*seed=*/input.front());
    for (size_t i = 0; i < input.size(); ++i) {
      std::string* val_ptr = test.Insert(input[i], "");
      ASSERT_THAT(val_ptr, NotNull());
      *val_ptr = vals[i];
    }
    for (size_t i = 0; i < input.size(); ++i) {
      std::string* res = test.Find(input[i]);
      ASSERT_THAT(res, NotNull());
      EXPECT_EQ(*res, vals[i]);
    }
    EXPECT_EQ(test.Size(), input.size());
  } while (std::next_permutation(input.begin(), input.end()));
}

TEST(TreapTest, InsertAndModifyOldValue) {
  constexpr std::array<int, 7> kInput{0, 2, 4, 6, 8, 10, 12};
  auto input = kInput;
  std::vector<std::string> vals{"a", "b", "c", "d", "e", "f", "g"};
  alpa::Treap<int, std::string> test(/*seed=*/kInput.size());
  for (size_t i = 0; i < input.size(); ++i) {
    test.Insert(input[i], vals[i]);
  }
  size_t count = 0;
  do {
    count++;
    std::string curr_val = std::to_string(count);
    for (size_t i = 0; i < input.size(); ++i) {
      std::string* val_ptr = test.Insert(input[i], "");
      EXPECT_EQ(test.Size(), input.size());
      ASSERT_THAT(val_ptr, NotNull());
      EXPECT_THAT(*val_ptr, Not(IsEmpty()));
      *val_ptr = curr_val;
    }
    for (const auto& val : input) {
      std::string* res = test.Find(val);
      ASSERT_THAT(res, NotNull());
      EXPECT_EQ(*res, curr_val);
    }
  } while (std::next_permutation(input.begin(), input.end()));
}

TEST(TreapTest, EraseFromEmpty) {
  alpa::Treap<int, int> test;
  EXPECT_TRUE(test.Empty());
  EXPECT_EQ(test.Size(), 0);
  EXPECT_FALSE(test.Erase(25));
  EXPECT_TRUE(test.Empty());
  EXPECT_EQ(test.Size(), 0);
  EXPECT_FALSE(test.Erase(0));
  EXPECT_FALSE(test.Erase(64));
  EXPECT_TRUE(test.Empty());
  EXPECT_EQ(test.Size(), 0);
}

TEST(TreapTest, EraseAllExisting) {
  constexpr std::array<int, 7> kInput{0, 1, 2, 3, 4, 5, 6};
  constexpr std::array<int, 7> kCheck = kInput;
  auto input = kInput;
  do {
    alpa::Treap<int, int> test(/*seed=*/static_cast<uint64_t>(input.back()));
    for (size_t i = 0; i < kInput.size(); ++i) {
      test.Insert(input[i], input[i] * input[i]);
    }
    size_t curr_size = input.size();
    EXPECT_EQ(test.Size(), curr_size);
    for (const auto& val : kCheck) {
      EXPECT_TRUE(test.Erase(val));
      EXPECT_EQ(test.Size(), --curr_size);
    }
    EXPECT_TRUE(test.Empty());
  } while (std::next_permutation(input.begin(), input.end()));
}

TEST(TreapTest, EraseNotExisting) {
  constexpr std::array<int, 7> kInput{0, 2, 4, 6, 8, 10, 12};
  constexpr std::array<int, 8> kCheck{-1, 1, 3, 5, 7, 9, 11, 13};
  auto input = kInput;
  do {
    alpa::Treap<int, int> test(/*seed=*/static_cast<uint64_t>(input.back()));
    for (const auto& val : input) {
      test.Insert(val, val * val);
    }
    for (const auto& val : kCheck) {
      EXPECT_FALSE(test.Erase(val));
      EXPECT_EQ(test.Size(), input.size());
    }
  } while (std::next_permutation(input.begin(), input.end()));
}
