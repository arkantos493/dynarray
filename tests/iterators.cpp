/**
 * Copyright (C) 2021-06-06 - Marcel Breyer - All Rights Reserved
 * Licensed under the MIT License. See LICENSE.md file in the project root for full license information.
 *
 * Implements tests for the iterator member functions of the cpp_util::dynarray class.
 */

#include "catch/catch.hpp"
#include "dynarray.hpp"

TEST_CASE("dynarray iterator member functions", "[iterator_support]") {
  cpp_util::dynarray<int> arr = {0, 1, 2, 3, 4, 5};
  const cpp_util::dynarray<int> const_arr = {0, 1, 2, 3, 4, 5};

  REQUIRE(arr.size() == 6);
  REQUIRE(const_arr.size() == 6);

  SECTION("begin()/cbegin() member functions") {
    CHECK(*arr.begin() == 0);
    CHECK(*arr.cbegin() == 0);
    CHECK(*const_arr.begin() == 0);
    CHECK(*const_arr.cbegin() == 0);
  }

  SECTION("end()/cend() member functions") {
    CHECK(*(arr.end() - 1) == 5);
    CHECK(*(arr.cend() - 1) == 5);
    CHECK(*(const_arr.end() - 1) == 5);
    CHECK(*(const_arr.cend() - 1) == 5);
  }

  SECTION("rbegin()/crbegin() member functions") {
    CHECK(*arr.rbegin() == 5);
    CHECK(*arr.crbegin() == 5);
    CHECK(*const_arr.rbegin() == 5);
    CHECK(*const_arr.crbegin() == 5);
  }

  SECTION("rend()/crend() member functions") {
    CHECK(*(arr.rend() - 1) == 0);
    CHECK(*(arr.crend() - 1) == 0);
    CHECK(*(const_arr.rend() - 1) == 0);
    CHECK(*(const_arr.crend() - 1) == 0);
  }
}
