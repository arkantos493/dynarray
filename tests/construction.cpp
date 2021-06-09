/**
 * Copyright (C) 2021-06-06 - Marcel Breyer - All Rights Reserved
 * Licensed under the MIT License. See LICENSE.md file in the project root for full license information.
 *
 * Implements tests for the constructors of the cpp_util::dynarray class.
 */

#include <algorithm>
#include <utility>

#include "catch/catch.hpp"
#include "dynarray.hpp"

TEST_CASE("dynarray constructors", "[construction]") {
  SECTION("default constructor") {
    cpp_util::dynarray<int> arr;

    CHECK(arr.empty());
    CHECK(arr.data() == nullptr);
  }

  SECTION("size constructor") {
    cpp_util::dynarray<int> arr(10);

    CHECK(arr.size() == 10);
  }

  SECTION("size + init value constructor") {
    cpp_util::dynarray<int> arr(10, 42);

    REQUIRE(arr.size() == 10);
    CHECK(std::all_of(arr.data(), arr.data() + 10, [](const int i) { return i == 42; }));
  }

  SECTION("iterator range constructor") {
    std::vector<int> vec(10, 42);
    cpp_util::dynarray<int> arr(vec.begin(), vec.end());

    REQUIRE(arr.size() == 10);
    CHECK(std::all_of(arr.data(), arr.data() + 10, [](const int i) { return i == 42; }));

#if __cplusplus >= 201703L
    // test deduction guide
    cpp_util::dynarray arr2(vec.begin(), vec.end());
#endif
  }

  SECTION("initializer_list constructor") {
    cpp_util::dynarray<int> arr = {42, 42, 42};

    REQUIRE(arr.size() == 3);
    CHECK(std::all_of(arr.data(), arr.data() + 3, [](const int i) { return i == 42; }));
  }

  SECTION("copy constructor") {
    cpp_util::dynarray<int> arr1 = {42, 42, 42};
    cpp_util::dynarray<int> arr2(arr1);

    REQUIRE(arr1.size() == arr2.size());
    CHECK(std::equal(arr1.data(), arr1.data() + 3, arr2.data(), arr2.data() + 3));
  }

  SECTION("move constructor") {
    cpp_util::dynarray<int> arr1 = {42, 42, 42};
    cpp_util::dynarray<int> arr2(std::move(arr1));

    CHECK(arr1.empty());
    CHECK(arr1.data() == nullptr);

    REQUIRE(arr2.size() == 3);
    CHECK(std::all_of(arr2.data(), arr2.data() + 3, [](const int i) { return i == 42; }));
  }
}