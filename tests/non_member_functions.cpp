/**
 * Copyright (C) 2021-06-06 - Marcel Breyer - All Rights Reserved
 * Licensed under the MIT License. See LICENSE.md file in the project root for full license information.
 *
 * Implements tests for the non-member functions of the cpp_util::dynarray class.
 */

#include "catch/catch.hpp"
#include "dynarray.hpp"

TEST_CASE("dynarray non-member functions", "[non-member]") {
  SECTION("swap() free function") {
    cpp_util::dynarray<int> arr1(10, 1);
    cpp_util::dynarray<int> arr2(5, 2);

    REQUIRE(arr1.size() == 10);
    REQUIRE(arr2.size() == 5);

    std::swap(arr1, arr2);

    REQUIRE(arr1.size() == 5);
    CHECK(std::all_of(arr1.data(), arr1.data() + 5, [](const int i) { return i == 2; }));
    REQUIRE(arr2.size() == 10);
    CHECK(std::all_of(arr2.data(), arr2.data() + 10, [](const int i) { return i == 1; }));
  }

  cpp_util::dynarray<int> arr1 = {1, 2, 3};
  cpp_util::dynarray<int> arr2 = {7, 8, 9, 10};
  cpp_util::dynarray<int> arr3 = {1, 2, 3};

  SECTION("compare non-equal") {
    CHECK_FALSE(arr1 == arr2);
    CHECK(arr1 != arr2);
    CHECK(arr1 < arr2);
    CHECK(arr1 <= arr2);
    CHECK_FALSE(arr1 > arr2);
    CHECK_FALSE(arr1 >= arr2);
  }

  SECTION("compare equal") {
    CHECK(arr1 == arr3);
    CHECK_FALSE(arr1 != arr3);
    CHECK_FALSE(arr1 < arr3);
    CHECK(arr1 <= arr3);
    CHECK_FALSE(arr1 > arr3);
    CHECK(arr1 >= arr3);
  }
}