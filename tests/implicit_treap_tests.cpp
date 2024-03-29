﻿#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "algorithm_pack/implicit_treap.h"

using ::testing::ElementsAre;
using ::testing::ElementsAreArray;

// Function is no-op and is provided in order to silence use-after-move warning
template <typename T>
void IsInitialized(T& /*unused*/) {}

TEST(ImplicitTreapTest, CreateEmpty) {
  alpa::ImplicitTreap<int> test;
  EXPECT_TRUE(test.Empty());
  EXPECT_EQ(test.Size(), 0);
  EXPECT_EQ(test.Begin(), test.End());
  alpa::ImplicitTreap<double> test2(/*seed=*/1);
  EXPECT_TRUE(test2.Empty());
  EXPECT_EQ(test2.Size(), 0);
  EXPECT_EQ(test2.Begin(), test2.End());
}

TEST(ImplicitTreapTest, SingleElement) {
  alpa::ImplicitTreap<int> test(/*seed=*/3);
  constexpr int kVal = 314;
  EXPECT_EQ(test.Insert(kVal, 0), kVal);
  EXPECT_FALSE(test.Empty());
  EXPECT_EQ(test.Size(), 1);
  ASSERT_NE(test.Begin(), test.End());
  EXPECT_EQ(*test.Begin(), kVal);
  *test.Begin() = kVal * kVal;
  EXPECT_EQ(*test.Begin(), kVal * kVal);
}

TEST(ImplicitTreapTest, PushBack) {
  constexpr std::array<int, 9> kInput{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test(kInput.size());
  for (const auto& el : kInput) {
    test.Insert(el, test.Size());
  }
  EXPECT_FALSE(test.Empty());
  EXPECT_EQ(test.Size(), kInput.size());
  EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
              ElementsAreArray(kInput));
}

TEST(ImplicitTreapTest, PushFront) {
  constexpr std::array<int, 9> kInput{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test(kInput.size());
  for (const auto& el : kInput) {
    test.Insert(el, 0);
  }
  EXPECT_FALSE(test.Empty());
  EXPECT_EQ(test.Size(), kInput.size());
  EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
              ElementsAreArray(kInput.rbegin(), kInput.rend()));
}

TEST(ImplicitTreapTest, InsertInside) {
  const std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  constexpr int kTestVal = 1024;
  for (size_t i = 1; i < input.size(); ++i) {
    alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
    test.Insert(kTestVal, /*pos=*/i);
    std::vector<int> res(test.Begin(), test.End());
    ASSERT_GT(res.size(), i);
    EXPECT_EQ(res[i], kTestVal) << " i = " << i;
  }
}

TEST(ImplicitTreapTest, IteratorWalk1) {
  const std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
  alpa::ImplicitTreap<int>::Iterator begin = test.Begin();
  auto cbegin = alpa::ImplicitTreap<int>::ConstIterator{begin};
  for (const auto& el : input) {
    EXPECT_EQ(el, *begin);
    EXPECT_EQ(*begin, *cbegin);
    ++begin;
    ++cbegin;
  }
}

TEST(ImplicitTreapTest, IteratorWalk2) {
  const std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
  alpa::ImplicitTreap<int>::Iterator begin = test.Begin();
  alpa::ImplicitTreap<int>::ConstIterator cbegin = test.CBegin();
  for (const auto& el : input) {
    EXPECT_EQ(el, *begin);
    EXPECT_EQ(*begin++, *cbegin++);
  }
}

TEST(ImplicitTreapTest, IteratorWalk3) {
  const std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
  alpa::ImplicitTreap<int>::Iterator last = --test.End();
  alpa::ImplicitTreap<int>::ConstIterator c_last = --test.CEnd();
  for (auto it = input.rbegin(); it != input.rend(); ++it) {
    EXPECT_EQ(*it, *last);
    EXPECT_EQ(*last, *c_last);
    --last;
    --c_last;
  }
}

TEST(ImplicitTreapTest, IteratorWalk4) {
  const std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
  alpa::ImplicitTreap<int>::Iterator last = --test.End();
  alpa::ImplicitTreap<int>::ConstIterator c_last = --test.CEnd();
  for (auto it = input.rbegin(); it != input.rend(); ++it) {
    EXPECT_EQ(*it, *last);
    EXPECT_EQ(*last--, *c_last--);
  }
}

TEST(ImplicitTreapTest, Constructors1) {
  alpa::ImplicitTreap<int> empty_treap;
  alpa::ImplicitTreap<int> test(std::move(empty_treap));
  EXPECT_TRUE(test.Empty());
  const std::vector<int> input{1, 2, 3, 4, 5};
  alpa::ImplicitTreap<int> test2(input, /*seed=*/input.size());
  test2 = std::move(test);
  EXPECT_TRUE(test2.Empty());
}

TEST(ImplicitTreapTest, Constructors2) {
  const std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> initial(input, /*seed=*/input.size());
  alpa::ImplicitTreap<int> test(std::move(initial));
  EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
              ElementsAreArray(input));
}

TEST(ImplicitTreapTest, Constructors3) {
  const std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
  const std::vector<int> input_2{10, 20};
  alpa::ImplicitTreap<int> test_2(input_2, /*seed=*/input_2.size());
  test_2 = std::move(test);
  EXPECT_THAT(std::vector<int>(test_2.Begin(), test_2.End()),
              ElementsAreArray(input));
}

TEST(ImplicitTreapTest, Constructor4) {
  // Move assignment to itself
  const std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
  test = std::move(test);  // NOLINT
  IsInitialized(test);
  EXPECT_EQ(test.Size(), input.size());
  EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
              ElementsAreArray(input));
}

TEST(ImplicitTreapTest, Constructors5) {
  alpa::ImplicitTreap<int> empty;
  alpa::ImplicitTreap<int> test(empty);
  EXPECT_TRUE(test.Empty());
  (void)test;  // Silencing warnings about not using test
  const std::vector<int> input{1, 2, 3, 4, 5};
  alpa::ImplicitTreap<int> test2(input, /*seed=*/input.size());
  test2 = empty;
  EXPECT_TRUE(test2.Empty());
}

TEST(ImplicitTreapTest, Constructors6) {
  const std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> initial(input, /*seed=*/input.size());
  alpa::ImplicitTreap<int> test(initial);
  EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
              ElementsAreArray(input));
  EXPECT_THAT(std::vector<int>(initial.Begin(), initial.End()),
              ElementsAreArray(input));
  EXPECT_EQ(test.Size(), initial.Size());
}

TEST(ImplicitTreapTest, Constructors7) {
  const std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> initial(input, /*seed=*/input.size());
  const std::vector<int> input_2{10, 20};
  alpa::ImplicitTreap<int> test(input_2, /*seed=*/input_2.size());
  test = initial;
  EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
              ElementsAreArray(input));
  EXPECT_THAT(std::vector<int>(initial.Begin(), initial.End()),
              ElementsAreArray(input));
  EXPECT_EQ(test.Size(), initial.Size());
}

TEST(ImplicitTreapTest, Constructor8) {
  // Copy assignment to itself
  const std::vector<int> input{1, 2, 3, 4, 5, 6, 7};
  alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
  test = test;  // NOLINT
  EXPECT_EQ(test.Size(), input.size());
  EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
              ElementsAreArray(input));
}

TEST(ImplicitTreapTest, ElementAccess1) {
  const std::vector<uint32_t> input{1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
  alpa::ImplicitTreap<uint32_t> test(input, /*seed=*/input.front());
  const alpa::ImplicitTreap<uint32_t> c_test(input, /*seed=*/input.back());
  for (size_t i = 0; i < input.size(); ++i) {
    EXPECT_EQ(test[i], input[i]);
    EXPECT_EQ(c_test[i], input[i]);
  }
}

TEST(ImplicitTreapTest, ElementAccess2) {
  const std::vector<int> data{1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
  auto input = data;
  alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
  constexpr int kSpecVal = 22564;
  for (size_t i = 0; i < test.Size(); ++i) {
    if ((i % 2) != 0) {
      test[i] = kSpecVal;
      input[i] = kSpecVal;
    }
  }
  EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
              ElementsAreArray(input));
}

TEST(ImplicitTreapTest, EraseFromBack) {
  const std::vector<int> data{1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto input = data;
  alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
  while (!input.empty()) {
    test.Erase(input.size() - 1);
    input.pop_back();
    EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
                ElementsAreArray(input));
  }
  EXPECT_TRUE(test.Empty());
}

TEST(ImplicitTreapTest, EraseFromFront) {
  const std::vector<int> data{1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto input = data;
  alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
  while (!input.empty()) {
    test.Erase(0);
    input.erase(input.begin());
    EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
                ElementsAreArray(input));
  }
  EXPECT_TRUE(test.Empty());
}

TEST(ImplicitTreapTest, EraseFromTheMiddle) {
  const std::vector<int> data{1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto input = data;
  alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
  while (!input.empty()) {
    size_t index = input.size() / 2;
    test.Erase(index);
    input.erase(input.begin() + static_cast<int>(index));
    EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
                ElementsAreArray(input));
  }
  EXPECT_TRUE(test.Empty());
}

TEST(ImplicitTreapTest, RotateFullVector) {
  const std::vector<int> data{1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto input = data;
  alpa::ImplicitTreap<int> treap(input, /*seed=*/input.size());
  treap.Rotate(0, 1, treap.Size());
  std::rotate(input.begin(), input.begin() + 1, input.end());
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));
  treap.Rotate(0, 4, treap.Size());
  std::rotate(input.begin(), input.begin() + 4, input.end());
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));
  treap.Rotate(0, treap.Size() - 1, treap.Size());
  std::rotate(input.begin(), input.end() - 1, input.end());
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));
}

TEST(ImplicitTreapTest, RotatePartOfTheVector) {
  const std::vector<int> data{1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto input = data;
  alpa::ImplicitTreap<int> treap(input, /*seed=*/input.size());
  // Rotate in the beginning
  {
    constexpr int kNewBegin = 1;
    constexpr int kEnd = 4;
    treap.Rotate(0, kNewBegin, kEnd);
    std::rotate(input.begin(), input.begin() + kNewBegin, input.begin() + kEnd);
    EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
                ElementsAreArray(input));
  }
  {
    constexpr int kNewBegin = 3;
    constexpr int kEnd = 4;
    treap.Rotate(0, kNewBegin, kEnd);
    std::rotate(input.begin(), input.begin() + kNewBegin, input.begin() + kEnd);
    EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
                ElementsAreArray(input));
  }
  {
    constexpr int kNewBegin = 2;
    constexpr int kEnd = 4;
    treap.Rotate(0, kNewBegin, kEnd);
    std::rotate(input.begin(), input.begin() + kNewBegin, input.begin() + kEnd);
    EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
                ElementsAreArray(input));
  }
  // Rotate in the middle
  {
    constexpr int kBegin = 2;
    constexpr int kNewBegin = 3;
    constexpr int kEnd = 6;
    treap.Rotate(kBegin, kNewBegin, kEnd);
    std::rotate(input.begin() + kBegin, input.begin() + kNewBegin,
                input.begin() + kEnd);
    EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
                ElementsAreArray(input));
  }
  {
    constexpr int kBegin = 2;
    constexpr int kNewBegin = 5;
    constexpr int kEnd = 6;
    treap.Rotate(kBegin, kNewBegin, kEnd);
    std::rotate(input.begin() + kBegin, input.begin() + kNewBegin,
                input.begin() + kEnd);
    EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
                ElementsAreArray(input));
  }
  {
    constexpr int kBegin = 2;
    constexpr int kNewBegin = 4;
    constexpr int kEnd = 6;
    treap.Rotate(kBegin, kNewBegin, kEnd);
    std::rotate(input.begin() + kBegin, input.begin() + kNewBegin,
                input.begin() + kEnd);
    EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
                ElementsAreArray(input));
  }
  // Rotate at the end
  {
    constexpr int kBegin = 2;
    constexpr int kNewBegin = 4;
    treap.Rotate(kBegin, kNewBegin, treap.Size());
    std::rotate(input.begin() + kBegin, input.begin() + kNewBegin, input.end());
    EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
                ElementsAreArray(input));
  }
  {
    constexpr int kBegin = 4;
    constexpr int kNewBegin = 6;
    treap.Rotate(kBegin, kNewBegin, treap.Size());
    std::rotate(input.begin() + kBegin, input.begin() + kNewBegin, input.end());
    EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
                ElementsAreArray(input));
  }
  {
    constexpr int kBegin = 4;
    const size_t new_begin = treap.Size() - 1;
    treap.Rotate(kBegin, new_begin, treap.Size());
    std::rotate(input.begin() + kBegin,
                input.begin() + static_cast<int>(new_begin), input.end());
    EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
                ElementsAreArray(input));
  }
  // Rotate whole
  {
    constexpr int kNewBegin = 4;
    treap.Rotate(0, kNewBegin, treap.Size());
    std::rotate(input.begin(), input.begin() + kNewBegin, input.end());
    EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
                ElementsAreArray(input));
  }
  {
    const size_t new_begin = treap.Size() - 1;
    treap.Rotate(0, new_begin, treap.Size());
    std::rotate(input.begin(), input.begin() + static_cast<int>(new_begin),
                input.end());
    EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
                ElementsAreArray(input));
  }
}

TEST(ImplicitTreapTest, RotateWithNoRotation) {
  const std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> treap(input, /*seed=*/input.size());
  treap.Rotate(0, 0, treap.Size());
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));
  const size_t mid_index = input.size() / 2;
  treap.Rotate(mid_index, mid_index, treap.Size());
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));
  treap.Rotate(treap.Size() - 1, treap.Size() - 1, treap.Size());
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));
  const size_t quarter_index = input.size() / 4;
  treap.Rotate(quarter_index, quarter_index, mid_index);
  EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
              ElementsAreArray(input));
}

TEST(ImplicitTreapTest, RotateCornerCases) {
  {  // Rotate empty treap
    alpa::ImplicitTreap<int> treap;
    EXPECT_TRUE(treap.Empty());
    EXPECT_EQ(treap.Size(), 0);
    treap.Rotate(0, 0, 0);
    EXPECT_TRUE(treap.Empty());
    EXPECT_EQ(treap.Size(), 0);
  }
  {
    const std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
    alpa::ImplicitTreap<int> treap(input, /*seed=*/input.size());
    // Rotate empty subrange in the beginning
    treap.Rotate(0, 0, 0);
    EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
                ElementsAreArray(input));
    // Rotate empty subrange in the middle
    const size_t middle_index = input.size() / 2;
    treap.Rotate(middle_index, middle_index, middle_index);
    EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
                ElementsAreArray(input));
    // Rotate empty subrange in the end
    const size_t last_index = input.size() - 1;
    treap.Rotate(last_index, last_index, last_index);
    EXPECT_THAT(std::vector<int>(treap.Begin(), treap.End()),
                ElementsAreArray(input));
  }
}

TEST(ImplicitTreapTest, IteratorInvalidationInsert) {
  const std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8};
  alpa::ImplicitTreap<int> test;
  std::vector<alpa::ImplicitTreap<int>::Iterator> iterators;
  iterators.reserve(input.size());
  std::vector<alpa::ImplicitTreap<int>::ConstIterator> const_iterators;
  const_iterators.reserve(input.size());
  auto end = test.End();
  auto cend = test.CEnd();
  for (const auto& input_el : input) {
    test.Insert(input_el, test.Size());
    if (iterators.empty()) {
      iterators.push_back(test.Begin());
      const_iterators.push_back(test.CBegin());
    } else {
      auto prev = iterators.back();
      iterators.push_back(++prev);
      auto c_prev = const_iterators.back();
      const_iterators.push_back(++c_prev);
    }
  }
  for (size_t i = 0; i < input.size(); ++i) {
    EXPECT_EQ(input[i], *iterators[i]);
    EXPECT_EQ(input[i], *const_iterators[i]);
  }
  auto res_end = iterators.back();
  auto c_res_end = const_iterators.back();
  EXPECT_EQ(++res_end, end);
  EXPECT_EQ(++c_res_end, cend);
}

TEST(ImplicitTreapTest, IteratorInvalidationRotate) {
  const std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
  auto it = test.Begin();
  auto cit = test.CBegin();
  auto end = test.End();
  auto cend = test.CEnd();
  test.Rotate(0, test.Size() - 2, test.Size());
  for (size_t i = 0; i < input.size() - 2; ++i) {
    EXPECT_EQ(*it++, input[i]);
    EXPECT_EQ(*cit++, input[i]);
  }
  EXPECT_EQ(it, end);
  EXPECT_EQ(cit, cend);
}

TEST(ImplicitTreapTest, IteratorInvalidationErase) {
  const std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8};
  alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
  auto it = test.Begin();
  auto cit = test.CBegin();
  auto end = test.End();
  auto cend = test.CEnd();
  size_t count = 0;
  for (size_t i = 0; i < input.size(); ++i) {
    if ((input[i] % 2) == 0) {
      test.Erase(i - count);
      ++count;
    }
  }
  for (const auto& input_el : input) {
    if ((input_el % 2) != 0) {
      EXPECT_EQ(*it++, input_el);
      EXPECT_EQ(*cit++, input_el);
    }
  }
  EXPECT_EQ(it, end);
  EXPECT_EQ(cit, cend);
}

TEST(ImplicitTreapTest, ConstructFromVector) {
  {
    const std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8};
    alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
    EXPECT_FALSE(test.Empty());
    EXPECT_EQ(test.Size(), input.size());
    EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
                ElementsAreArray(input));
  }
  {
    std::vector<int> input{};
    alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
    EXPECT_TRUE(test.Empty());
    EXPECT_EQ(test.Size(), 0);
  }
  {
    const std::vector<int> input{25};
    alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
    EXPECT_FALSE(test.Empty());
    EXPECT_EQ(test.Size(), 1);
    EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
                ElementsAreArray(input));
  }
}

TEST(ImplicitTreapTest, IteratorCall) {
  const std::vector<std::string> input{"one", "two", "three"};
  alpa::ImplicitTreap<std::string> test(input, /*seed=*/input.size());
  auto it = test.Begin();
  auto cit = test.CBegin();
  auto exp_it = input.begin();
  while (exp_it != input.end()) {
    EXPECT_EQ(it->find("e"), exp_it->find("e"));
    EXPECT_EQ(cit->find("e"), exp_it->find("e"));
    ++it;
    ++cit;
    ++exp_it;
  }
}

TEST(ImplicitTreapTest, Concatenate1) {
  {  // General case
    const std::vector<uint32_t> input{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto mid_it = input.begin() + static_cast<int>(input.size() / 2);
    alpa::ImplicitTreap<uint32_t> lhs{{input.begin(), mid_it},
                                      /*seed=*/input.front()};
    alpa::ImplicitTreap<uint32_t> rhs{{mid_it, input.end()},
                                      /*seed=*/input.back()};
    lhs.Concatenate(std::move(rhs));
    EXPECT_EQ(lhs.Size(), input.size());
    EXPECT_THAT(std::vector<int>(lhs.Begin(), lhs.End()),
                ElementsAreArray(input));
  }
  {  // Concatenate empty
    const std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    alpa::ImplicitTreap<int> lhs{input, /*seed=*/input.size()};
    alpa::ImplicitTreap<int> rhs;
    lhs.Concatenate(std::move(rhs));
    EXPECT_EQ(lhs.Size(), input.size());
    EXPECT_THAT(std::vector<int>(lhs.Begin(), lhs.End()),
                ElementsAreArray(input));
  }
  {  // Concatenate to empty
    const std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    alpa::ImplicitTreap<int> lhs;
    alpa::ImplicitTreap<int> rhs{input, /*seed=*/input.size()};
    lhs.Concatenate(std::move(rhs));
    EXPECT_EQ(lhs.Size(), input.size());
    EXPECT_THAT(std::vector<int>(lhs.Begin(), lhs.End()),
                ElementsAreArray(input));
  }
}

TEST(ImplicitTreapTest, Concatenate2) {
  const std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  alpa::ImplicitTreap<int> test;
  test.Insert(0, /*pos=*/0);
  auto it = test.Begin();
  auto cit = test.CBegin();
  auto end = test.End();
  auto cend = test.CEnd();
  int prev_val = 0;
  for (const auto& el : input) {
    test.Concatenate(alpa::ImplicitTreap<int>({el}, /*seed=*/input.size()));
    ++it;
    ++cit;
    EXPECT_EQ(*it, el);
    EXPECT_EQ(*cit, el);
    --it;
    --cit;
    EXPECT_EQ(*it, prev_val);
    EXPECT_EQ(*cit, prev_val);
    ++it;
    ++cit;
    prev_val = el;
  }
  EXPECT_EQ(test.End(), end);
  EXPECT_EQ(test.CEnd(), cend);
}

TEST(ImplicitTreapTest, RandomAccessIteratorShifts1) {
  const std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8};
  alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
  {
    auto it = test.Begin();
    auto cit = test.CBegin();
    EXPECT_EQ(*(it + 1), input[1]);
    EXPECT_EQ(*(cit + 1), input[1]);
    EXPECT_EQ(*(1 + it), input[1]);
    EXPECT_EQ(*(1 + cit), input[1]);
    EXPECT_EQ(*(it + 0), input[0]);
    EXPECT_EQ(*(cit + 0), input[0]);
    EXPECT_EQ(*(0 + it), input[0]);
    EXPECT_EQ(*(0 + cit), input[0]);
    EXPECT_EQ(*(it + 3), input[3]);
    EXPECT_EQ(*(cit + 3), input[3]);
    EXPECT_EQ(*(3 + it), input[3]);
    EXPECT_EQ(*(3 + cit), input[3]);
    int last_index = static_cast<int>(input.size()) - 1;
    EXPECT_EQ(*(it + last_index), input.back());
    EXPECT_EQ(*(cit + last_index), input.back());
    EXPECT_EQ(*(last_index + it), input.back());
    EXPECT_EQ(*(last_index + cit), input.back());
    EXPECT_EQ(it + static_cast<int>(input.size()), test.End());
    EXPECT_EQ(cit + static_cast<int>(input.size()), test.CEnd());
    EXPECT_EQ(static_cast<int>(input.size()) + it, test.End());
    EXPECT_EQ(static_cast<int>(input.size()) + cit, test.CEnd());
  }
  {
    auto it = test.End();
    auto cit = test.CEnd();
    EXPECT_EQ(*(it - 1), input.back());
    EXPECT_EQ(*(cit - 1), input.back());
    EXPECT_EQ(*(it - 3), input[5]);
    EXPECT_EQ(*(cit - 3), input[5]);
    int last_index = static_cast<int>(input.size()) - 1;
    EXPECT_EQ(*(it - last_index), input[1]);
    EXPECT_EQ(*(cit - last_index), input[1]);
    EXPECT_EQ(*(it - static_cast<int>(input.size())), input.front());
    EXPECT_EQ(*(cit - static_cast<int>(input.size())), input.front());
  }
  {
    auto it = test.Begin() + 3;
    auto cit = test.CBegin() + 3;
    EXPECT_EQ(*(it - 0), input[3]);
    EXPECT_EQ(*(cit - 0), input[3]);
    EXPECT_EQ(*(it + 0), input[3]);
    EXPECT_EQ(*(cit + 0), input[3]);
    EXPECT_EQ(*(it - 1), input[2]);
    EXPECT_EQ(*(cit - 1), input[2]);
    EXPECT_EQ(*(it + 1), input[4]);
    EXPECT_EQ(*(cit + 1), input[4]);
    EXPECT_EQ(*(it - 3), input[0]);
    EXPECT_EQ(*(cit - 3), input[0]);
    EXPECT_EQ(*(it + 3), input[6]);
    EXPECT_EQ(*(cit + 3), input[6]);
  }
}

TEST(ImplicitTreapTest, RandomAccessIteratorShifts2) {
  // test jump from different points in the tree to some other different points
  // in the tree
  constexpr int kInputSize = 10'000;
  std::vector<int> input(kInputSize);
  std::iota(input.begin(), input.end(), 0);
  alpa::ImplicitTreap<int> test(input, /*seed=*/kInputSize);
  std::mt19937 rnd(input.size());
  std::uniform_int_distribution<int> dist(0,
                                          static_cast<int>(input.size()) - 1);
  constexpr int kCount = 500;
  auto it = test.Begin();
  auto cit = test.CBegin();
  for (int i = 0; i < kCount; ++i) {
    int index_from = dist(rnd);
    int index_to = dist(rnd);
    int shift = index_to - index_from;
    EXPECT_EQ(*((it + index_from) + shift),
              input[static_cast<size_t>(index_to)]);
    EXPECT_EQ(*((cit + index_from) + shift),
              input[static_cast<size_t>(index_to)]);
  }
}

TEST(ImplicitTreapTest, RandomAccessIteratorDifference) {
  constexpr int kInputSize = 10'000;
  std::vector<int> input(kInputSize);
  std::iota(input.begin(), input.end(), 0);
  alpa::ImplicitTreap<int> test(input, /*seed=*/kInputSize);
  std::mt19937 rnd(input.size());
  std::uniform_int_distribution<int> dist(0,
                                          static_cast<int>(input.size()) - 1);
  constexpr int kCount = 500;
  auto it = test.Begin();
  auto cit = test.CBegin();
  for (int i = 0; i < kCount; ++i) {
    int index_from = dist(rnd);
    int index_to = dist(rnd);
    int expected = index_to - index_from;
    auto it_from = it + index_from;
    auto it_to = it + index_to;
    auto cit_from = cit + index_from;
    auto cit_to = cit + index_to;
    EXPECT_EQ(it_to - it_from, expected);
    EXPECT_EQ(cit_to - cit_from, expected);
    EXPECT_EQ(cit_to - alpa::ImplicitTreap<int>::ConstIterator{it_from},
              expected);
    EXPECT_EQ(alpa::ImplicitTreap<int>::ConstIterator(it_to) - cit_from,
              expected);
  }
  EXPECT_EQ(test.End() - it, test.Size());
  EXPECT_EQ(alpa::ImplicitTreap<int>::ConstIterator{test.End()} - cit,
            test.Size());
  EXPECT_EQ(test.CEnd() - cit, test.Size());
  EXPECT_EQ(alpa::ImplicitTreap<int>::ConstIterator{test.End()} - cit,
            test.Size());
}

TEST(ImplicitTreapTest, Extract1) {
  {  // Extract entire range
    const std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
    alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
    alpa::ImplicitTreap<int> res = test.Extract(0, test.Size());
    EXPECT_EQ(test.Size(), 0);
    EXPECT_TRUE(test.Empty());
    EXPECT_EQ(res.Size(), input.size());
    EXPECT_FALSE(res.Empty());
    EXPECT_THAT(std::vector<int>(res.Begin(), res.End()),
                ElementsAreArray(input));
  }
  {  // Extract empty range
    const std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
    alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
    alpa::ImplicitTreap<int> res =
        test.Extract(input.size() / 2, input.size() / 2);
    EXPECT_EQ(res.Size(), 0);
    EXPECT_TRUE(res.Empty());
    EXPECT_EQ(test.Size(), input.size());
    EXPECT_FALSE(test.Empty());
    EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
                ElementsAreArray(input));
  }
  {  // Extract single element
    const std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
    constexpr int kExtractBegin = 5;
    constexpr int kExtractEnd = 6;
    alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
    alpa::ImplicitTreap<int> res = test.Extract(kExtractBegin, kExtractEnd);
    EXPECT_EQ(res.Size(), 1);
    EXPECT_FALSE(res.Empty());
    EXPECT_EQ(test.Size(), input.size() - 1);
    EXPECT_FALSE(test.Empty());
    EXPECT_THAT(std::vector<int>(res.Begin(), res.End()), ElementsAre(6));
    EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
                ElementsAre(1, 2, 3, 4, 5, 7, 8, 9));
  }
  {  // Extract empty from the empty
    alpa::ImplicitTreap<int> test(/*seed=*/0);
    alpa::ImplicitTreap<int> res = test.Extract(0, 0);
    EXPECT_EQ(test.Size(), 0);
    EXPECT_TRUE(test.Empty());
    EXPECT_EQ(res.Size(), 0);
    EXPECT_TRUE(res.Empty());
  }
  {  // Extract from the beginning
    const std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
    constexpr int kExtractBegin = 0;
    constexpr int kExtractEnd = 4;
    alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
    alpa::ImplicitTreap<int> start = test.Extract(kExtractBegin, kExtractEnd);
    EXPECT_EQ(start.Size(), kExtractEnd - kExtractBegin);
    EXPECT_EQ(test.Size(), input.size() - (kExtractEnd - kExtractBegin));
    EXPECT_THAT(std::vector<int>(start.Begin(), start.End()),
                ElementsAreArray(input.begin() + kExtractBegin,
                                 input.begin() + kExtractEnd));
    EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
                ElementsAreArray(input.begin() + kExtractEnd, input.end()));
  }
  {  // Extract from the end
    const std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
    constexpr int kExtractBegin = 6;
    alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
    alpa::ImplicitTreap<int> end = test.Extract(kExtractBegin, input.size());
    EXPECT_EQ(end.Size(), input.size() - kExtractBegin);
    EXPECT_FALSE(end.Empty());
    EXPECT_EQ(test.Size(), kExtractBegin);
    EXPECT_FALSE(test.Empty());
    EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
                ElementsAreArray(input.begin(), input.begin() + kExtractBegin));
    EXPECT_THAT(std::vector<int>(end.Begin(), end.End()),
                ElementsAreArray(input.begin() + kExtractBegin, input.end()));
  }
  {  // Extract from the middle
    const std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
    constexpr int kMiddleBegin = 4;
    constexpr int kMiddleEnd = 7;
    alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
    alpa::ImplicitTreap<int> middle = test.Extract(kMiddleBegin, kMiddleEnd);
    EXPECT_EQ(middle.Size(), kMiddleEnd - kMiddleBegin);
    EXPECT_FALSE(middle.Empty());
    EXPECT_EQ(test.Size(), input.size() - (kMiddleEnd - kMiddleBegin));
    EXPECT_FALSE(test.Empty());
    EXPECT_THAT(std::vector<int>(middle.Begin(), middle.End()),
                ElementsAreArray(input.begin() + kMiddleBegin,
                                 input.begin() + kMiddleEnd));
    std::vector<int> reminder(input.begin(), input.begin() + kMiddleBegin);
    reminder.insert(reminder.end(), input.begin() + kMiddleEnd, input.end());
    EXPECT_THAT(std::vector<int>(test.Begin(), test.End()),
                ElementsAreArray(reminder));
  }
}

TEST(ImplicitTreapTest, Extract2) {
  // testing iterator invalidation
  const std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  constexpr int kExtractBegin = 4;
  constexpr int kExtractEnd = 7;
  const std::vector<int> expected{1, 2, 3, 4, 8, 9};
  alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
  auto it = test.Begin();
  auto cit = test.CBegin();
  auto end = test.End();
  auto cend = test.CEnd();
  alpa::ImplicitTreap<int> extracted = test.Extract(kExtractBegin, kExtractEnd);
  for (const auto& el : expected) {
    EXPECT_EQ(*it++, el);
    EXPECT_EQ(*cit++, el);
  }
  EXPECT_EQ(it, end);
  EXPECT_EQ(cit, cend);
}

TEST(ImplicitTreapTest, StlMethodCalls) {
  const std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8, 9};
  constexpr int kSearchVal = 6;
  constexpr int kNextAfterSearch = 7;
  constexpr int kPrevAfterSearch = 5;
  alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
  std::vector<int> result(test.Begin(), test.End());
  EXPECT_THAT(result, ElementsAreArray(input));
  std::vector<int> c_result(test.CBegin(), test.CEnd());
  EXPECT_THAT(c_result, ElementsAreArray(input));
  auto cit = std::lower_bound(test.CBegin(), test.CEnd(), kSearchVal);
  auto it = std::lower_bound(test.Begin(), test.End(), kSearchVal);
  ASSERT_NE(cit, test.CEnd());
  ASSERT_NE(it, test.End());
  EXPECT_EQ(*cit, kSearchVal);
  EXPECT_EQ(*std::next(cit), kNextAfterSearch);
  EXPECT_EQ(*it, kSearchVal);
  EXPECT_EQ(*std::prev(it), kPrevAfterSearch);
}

TEST(ImplicitTreapTest, SwapTest) {
  const std::vector<uint32_t> input_1{1, 2, 3, 4, 5, 6};
  const std::vector<uint32_t> input_2{6, 5, 4, 3, 2, 1};
  alpa::ImplicitTreap<uint32_t> lhs{input_1, /*seed=*/input_1.back()};
  alpa::ImplicitTreap<uint32_t> rhs{input_2, /*seed=*/input_2.back()};
  lhs.Swap(rhs);
  EXPECT_THAT(std::vector<uint32_t>(lhs.Begin(), lhs.End()),
              ElementsAreArray(input_2));
  EXPECT_THAT(std::vector<uint32_t>(rhs.Begin(), rhs.End()),
              ElementsAreArray(input_1));
  rhs.Swap(rhs);
  EXPECT_THAT(std::vector<uint32_t>(rhs.Begin(), rhs.End()),
              ElementsAreArray(input_1));
}

TEST(ImplicitTreapTest, ClearTest) {
  const std::vector<int> input{1, 2, 3, 4, 5, 6, 7, 8};
  alpa::ImplicitTreap<int> test(input, /*seed=*/input.size());
  test.Clear();
  EXPECT_EQ(test.Size(), 0);
  EXPECT_TRUE(test.Empty());
  test.Insert(input.front(), /*pos=*/0);
  test.Insert(input.back(), /*pos=*/0);
  alpa::ImplicitTreap<int> dest(std::move(test));
  IsInitialized(test);
  test.Clear();
  EXPECT_EQ(test.Size(), 0);
  EXPECT_TRUE(test.Empty());
}