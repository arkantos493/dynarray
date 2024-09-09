/**
 * Copyright (C) 2021 - Marcel Breyer - All Rights Reserved
 * Licensed under the MIT License. See LICENSE.md file in the project root for full license information.
 *
 * Implements tests for the non-member functions of the cpp_util::dynarray class.
 */

#include "dynarray.hpp"

#include "catch/catch.hpp"

#include <algorithm>  // std::swap, std::all_of

TEST_CASE("dynarray non-member functions", "[non-member]") {
    SECTION("swap() free function") {
        cpp_util::dynarray<int> arr1(10, 1);
        cpp_util::dynarray<int> arr2(5, 2);

        REQUIRE(arr1.size() == 10);
        REQUIRE(arr2.size() == 5);

        cpp_util::swap(arr1, arr2);

        REQUIRE(arr1.size() == 5);
        CHECK(std::all_of(arr1.begin(), arr1.end(), [](const int i) { return i == 2; }));
        REQUIRE(arr2.size() == 10);
        CHECK(std::all_of(arr2.begin(), arr2.end(), [](const int i) { return i == 1; }));

        std::swap(arr1, arr2);

        REQUIRE(arr1.size() == 10);
        CHECK(std::all_of(arr1.begin(), arr1.end(), [](const int i) { return i == 1; }));
        REQUIRE(arr2.size() == 5);
        CHECK(std::all_of(arr2.begin(), arr2.end(), [](const int i) { return i == 2; }));
    }

    const cpp_util::dynarray<int> arr1 = { 1, 2, 3 };
    const cpp_util::dynarray<int> arr2 = { 7, 8, 9, 10 };
    const cpp_util::dynarray<int> arr3 = { 1, 2, 3 };

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
