/**
 * Copyright (C) 2021 - Marcel Breyer - All Rights Reserved
 * Licensed under the MIT License. See LICENSE.md file in the project root for full license information.
 *
 * Implements tests for the element access member functions of the cpp_util::dynarray class.
 */

#include "dynarray.hpp"

#include "catch/catch.hpp"

#include <cstddef>    // std::size_t
#include <stdexcept>  // std::out_of_range

TEST_CASE("dynarray element access member functions", "[element_access]") {
    cpp_util::dynarray<std::size_t> arr = { 0, 1, 2, 3, 4, 5 };
    const cpp_util::dynarray<std::size_t> const_arr = { 0, 1, 2, 3, 4, 5 };
    cpp_util::dynarray<std::size_t> default_arr;

    REQUIRE(arr.size() == 6);
    REQUIRE(const_arr.size() == 6);

    SECTION("at() member function") {
        for (std::size_t i = 0; i < 6; ++i) {
            CHECK(arr.at(i) == i);
            CHECK(const_arr.at(i) == i);
        }

        CHECK_THROWS_AS(arr.at(6), std::out_of_range);
        CHECK_THROWS_AS(const_arr.at(6), std::out_of_range);
        CHECK_THROWS_AS(default_arr.at(0), std::out_of_range);
    }

    SECTION("operator[] member function") {
        for (std::size_t i = 0; i < 6; ++i) {
            CHECK(arr[i] == i);
            CHECK(const_arr[i] == i);
        }
    }

    SECTION("front() member function") {
        CHECK(arr.front() == 0);
        CHECK(const_arr.front() == 0);
    }

    SECTION("back() member function") {
        CHECK(arr.back() == 5);
        CHECK(const_arr.back() == 5);
    }

    SECTION("data() member function") {
        CHECK(arr.data() == &arr[0]);
        CHECK(const_arr.data() == &const_arr[0]);
    }
}
