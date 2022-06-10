#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <utility>
#include <vector>

#include "algorithm_pack/implicit_treap.h"

using ::testing::ElementsAreArray;

namespace {
template <typename T>
alpa::ImplicitTreap<T> CreateTreap(const std::vector<T>& input,
                                   uint64_t seed = 314) {
  alpa::ImplicitTreap<T> res(seed);
  for (const auto& el : input) {
    res.Insert(el, res.Size());
  }
  return res;
}
}  // namespace

TEST(ImplicitTreapTest, CreateEmpty) {
  alpa::ImplicitTreap<int> test;
  EXPECT_TRUE(test.Empty());
  EXPECT_EQ(test.Size(), 0);
  EXPECT_EQ(test.Begin(), test.End());
  alpa::ImplicitTreap<double> test2(35);
  EXPECT_TRUE(test2.Empty());
  EXPECT_EQ(test2.Size(), 0);
  EXPECT_EQ(test2.Begin(), test2.End());
}

TEST(ImplicitTreapTest, SingleElement) {
  alpa::ImplicitTreap<int> test(42);
  int val = 314;
  EXPECT_EQ(test.Insert(val, 0), val);
  EXPECT_FALSE(test.Empty());
  EXPECT_EQ(test.Size(), 1);
  ASSERT_NE(test.Begin(), test.End());
  EXPECT_EQ(*test.Begin(), val);
  *test.Begin() = 24;
  EXPECT_EQ(*test.Begin(), 24);
}

TEST(ImplicitTreapTest, PushBack) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test(42);
  for (const auto& el : input) {
    test.Insert(el, test.Size());
  }
  EXPECT_FALSE(test.Empty());
  EXPECT_EQ(test.Size(), input.size());
  EXPECT_THAT(test.ConvertToVector(), ElementsAreArray(input));
}

TEST(ImplicitTreapTest, PushFront) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test(42);
  for (const auto& el : input) {
    test.Insert(el, 0);
  }
  EXPECT_FALSE(test.Empty());
  EXPECT_EQ(test.Size(), input.size());
  EXPECT_THAT(test.ConvertToVector(),
              ElementsAreArray(input.rbegin(), input.rend()));
}

TEST(ImplicitTreapTest, InsertInside) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (size_t i = 1; i < input.size(); ++i) {
    alpa::ImplicitTreap<int> test = CreateTreap(input);
    test.Insert(1024, i);
    std::vector<int> res = test.ConvertToVector();
    ASSERT_GT(res.size(), i);
    EXPECT_EQ(res[i], 1024) << " i = " << i;
  }
}

TEST(ImplicitTreapTest, IteratorWalk1) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> x = CreateTreap(input);
  alpa::ImplicitTreap<int>::Iterator begin = x.Begin();
  alpa::ImplicitTreap<int>::ConstIterator cbegin = begin;
  for (const auto& el : input) {
    EXPECT_EQ(el, *begin);
    EXPECT_EQ(*begin, *cbegin);
    ++begin;
    ++cbegin;
  }
}

TEST(ImplicitTreapTest, IteratorWalk2) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> x = CreateTreap(input);
  alpa::ImplicitTreap<int>::Iterator begin = x.Begin();
  alpa::ImplicitTreap<int>::ConstIterator cbegin = x.CBegin();
  for (const auto& el : input) {
    EXPECT_EQ(el, *begin);
    EXPECT_EQ(*begin++, *cbegin++);
  }
}

TEST(ImplicitTreapTest, IteratorWalk3) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> x = CreateTreap(input);
  alpa::ImplicitTreap<int>::Iterator last = --x.End();
  alpa::ImplicitTreap<int>::ConstIterator clast = --x.CEnd();
  for (auto it = input.rbegin(); it != input.rend(); ++it) {
    EXPECT_EQ(*it, *last);
    EXPECT_EQ(*last, *clast);
    --last;
    --clast;
  }
}

TEST(ImplicitTreapTest, IteratorWalk4) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> x = CreateTreap(input);
  alpa::ImplicitTreap<int>::Iterator last = --x.End();
  alpa::ImplicitTreap<int>::ConstIterator clast = --x.CEnd();
  for (auto it = input.rbegin(); it != input.rend(); ++it) {
    EXPECT_EQ(*it, *last);
    EXPECT_EQ(*last--, *clast--);
  }
}

TEST(ImplicitTreapTest, Constructors1) {
  alpa::ImplicitTreap<int> x;
  alpa::ImplicitTreap<int> test(std::move(x));
  EXPECT_TRUE(test.Empty());
  alpa::ImplicitTreap<int> test2 = std::move(test);
  EXPECT_TRUE(test2.Empty());
}

TEST(ImplicitTreapTest, Constructors2) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> x = CreateTreap(input);
  alpa::ImplicitTreap<int> test(std::move(x));
  EXPECT_THAT(test.ConvertToVector(), ElementsAreArray(input));
}

TEST(ImplicitTreapTest, Constructors3) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> x = CreateTreap(input);
  alpa::ImplicitTreap<int> test = CreateTreap(std::vector<int>{10, 20});
  test = std::move(x);
  EXPECT_THAT(test.ConvertToVector(), ElementsAreArray(input));
}

TEST(ImplicitTreapTest, ElementAccess1) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
  alpa::ImplicitTreap<int> test = CreateTreap(input);
  const alpa::ImplicitTreap<int> ctest = CreateTreap(input);
  for (size_t i = 0; i < input.size(); ++i) {
    EXPECT_EQ(test[i], input[i]);
    EXPECT_EQ(ctest[i], input[i]);
  }
}

TEST(ImplicitTreapTest, ElementAccess2) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
  alpa::ImplicitTreap<int> test = CreateTreap(input);
  int special_val = 22564;
  for (size_t i = 0; i < test.Size(); ++i) {
    if (i % 2) {
      test[i] = special_val;
      input[i] = special_val;
    }
  }
  EXPECT_THAT(test.ConvertToVector(), ElementsAreArray(input));
}

TEST(ImplicitTreapTest, EraseFromBack) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test = CreateTreap(input);
  while (!input.empty()) {
    test.Erase(input.size() - 1);
    input.pop_back();
    EXPECT_THAT(test.ConvertToVector(), ElementsAreArray(input));
  }
  EXPECT_TRUE(test.Empty());
}

TEST(ImplicitTreapTest, EraseFromFront) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test = CreateTreap(input);
  while (!input.empty()) {
    test.Erase(0);
    input.erase(input.begin());
    EXPECT_THAT(test.ConvertToVector(), ElementsAreArray(input));
  }
  EXPECT_TRUE(test.Empty());
}

TEST(ImplicitTreapTest, EraseFromTheMiddle) {
  std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test = CreateTreap(input);
  while (!input.empty()) {
    size_t index = input.size() / 2;
    test.Erase(index);
    input.erase(input.begin() + index);
    EXPECT_THAT(test.ConvertToVector(), ElementsAreArray(input));
  }
  EXPECT_TRUE(test.Empty());
}
