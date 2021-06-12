#include <array>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <random>

#include "dynarray.hpp"

#if __cplusplus >= 202002L
#include <algorithm>

constexpr int gaus_sum(const std::size_t n) {
  cpp_util::dynarray<int> arr(n);
  arr.iota(0);
  return std::accumulate(arr.begin(), arr.end(), 0);
}
#endif

int main() {
  {
    cpp_util::dynarray<int> arr1;
    cpp_util::dynarray<int> arr2(10);
    cpp_util::dynarray<int> arr3(10, 4);
    cpp_util::dynarray<int> arr4 = {1, 2, 3, 4, 5};

    std::vector<float> vec = {1, 2, 3, 4, 5};
    cpp_util::dynarray<float> arr5(vec.begin(), vec.end());

    cpp_util::dynarray<int> arr6 = arr3;
    cpp_util::dynarray<int> arr7 = std::move(arr3);
  }

  cpp_util::dynarray<int> arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  std::cout << arr[1] << std::endl;
  std::cout << arr.at(2) << std::endl;
  std::cout << arr.front() << std::endl;
  std::cout << arr.back() << std::endl;

  for (const int i : arr) {
    std::cout << i << ' ';
  }
  std::cout << std::endl;
  for (auto it = arr.rbegin(), end = arr.rend(); it != end; ++it) {
    std::cout << *it << ' ';
  }
  std::cout << std::endl;

  std::cout << std::boolalpha;
  std::cout << arr.empty() << ' ' << cpp_util::dynarray<int>{}.empty() << std::endl;
  std::cout << arr.size() << std::endl;

  arr.fill(4);
  for (const int i : arr) {
    std::cout << i << ' ';
  }
  std::cout << std::endl;
  arr.iota(10);
  for (const int i : arr) {
    std::cout << i << ' ';
  }
  std::cout << std::endl;
  auto rand = []() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> dist(1, 10);
    return dist(gen);
  };
  arr.generate(rand);
  for (const int i : arr) {
    std::cout << i << ' ';
  }
  std::cout << std::endl;

  {
    cpp_util::dynarray<int> arr1 = {1, 2, 3, 4};
    cpp_util::dynarray<int> arr2 = arr1;
    cpp_util::dynarray<int> arr3 = {1, 2};
    cpp_util::dynarray<int> arr4 = {2, 3, 4, 5};

    std::cout << std::endl;
    std::cout << (arr1 == arr2) << ' ' << (arr1 == arr3) << ' ' << (arr1 == arr4) << std::endl;
    std::cout << (arr1 != arr2) << ' ' << (arr1 != arr3) << ' ' << (arr1 != arr4) << std::endl;
    std::cout << (arr1 < arr2) << ' ' << (arr1 < arr3) << ' ' << (arr1 < arr4) << std::endl;
    std::cout << (arr1 > arr2) << ' ' << (arr1 > arr3) << ' ' << (arr1 > arr4) << std::endl;
    std::cout << (arr1 <= arr2) << ' ' << (arr1 <= arr3) << ' ' << (arr1 <= arr4) << std::endl;
    std::cout << (arr1 >= arr2) << ' ' << (arr1 >= arr3) << ' ' << (arr1 >= arr4) << std::endl;
  }

#if __cplusplus >= 202002L
  // constexpr test
  std::array<int, gaus_sum(10)> std_arr;
  std::cout << std_arr << std::endl;
#endif

  return 0;
}
