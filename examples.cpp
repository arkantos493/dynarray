#include "dynarray.hpp"

#include <exception>  // std::exception
#include <iostream>   // std::cout, std::boolalpha
#include <iterator>
#include <random>     // std::random_device, std::mt19937, std::uniform_int_distribution
#include <utility>    // std::move
#include <vector>     // std::vector


#if defined(__cpp_constexpr_dynamic_alloc) && defined(__cpp_lib_constexpr_numeric)
#include <algorithm>  // std::accumulate

constexpr int gaus_sum(const std::size_t n) {
  cpp_util::dynarray<int> arr(n);
  arr.iota(0);
  return std::accumulate(arr.begin(), arr.end(), 0);
}

template <int val>
int force_compile_time_evaluation() { return val; }
#endif


// helper function for printing a dynarray
template <typename T>
void print(const cpp_util::dynarray<T>& arr) {
  for (const auto val : arr) {
    std::cout << val << ' ';
  }
  std::cout << "\n\n";
}


int main() {
  // different constructors
  {
    cpp_util::dynarray<int> arr1;
    cpp_util::dynarray<int> arr2(10);
    cpp_util::dynarray<int> arr3(10, 4);
    cpp_util::dynarray<int> arr4 = {1, 2, 3, 4, 5};

    std::vector<int> vec = {1, 2, 3, 4, 5};
    cpp_util::dynarray<int> arr5(vec.begin(), vec.end());

    cpp_util::dynarray<int> arr6 = arr3;
    cpp_util::dynarray<int> arr7 = std::move(arr3);
  }

  // array access functions
  {
    cpp_util::dynarray<int> acc_arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::cout << acc_arr[1] << ' ' << acc_arr.at(2) << ' ' << acc_arr.front() << ' ' << acc_arr.back() << "\n\n";

    try {
      std::cout << acc_arr.at(11) << '\n';
    } catch (const std::exception& e) {
      std::cout << e.what() << '\n';
    }
  }


  // capacity functions
  {
    cpp_util::dynarray<int> cap_arr(100);
    std::cout << std::boolalpha;
    std::cout << cap_arr.empty() << ' ' << cpp_util::dynarray<int>{}.empty() << ' ' << cap_arr.size() << "\n\n";
  }

  // iterator support
  {
      cpp_util::dynarray<double> iter_arr = {1.1, 2.2, 3.3, 4.4, 5.5};
      for (auto it = iter_arr.begin(), end = iter_arr.end(); it != end; ++it) {
        *it += 6.6;
      }
      print(iter_arr);
  }

  // create and fill array with iota values
  {
    cpp_util::dynarray<int> iota_arr(10);
    iota_arr.iota();
    print(iota_arr);
  }

  // create dynarray and fill it with random values
  {
    // create random distribution
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 42);
    // create and fill array
    cpp_util::dynarray<int> rand_arr(10);
    rand_arr.generate([&]() { return dist(gen); });
    print(rand_arr);
  }

  // compare two dynarrays
  {
    cpp_util::dynarray<int> alice = {1, 2, 3};
    cpp_util::dynarray<int> bob = {7, 8, 9, 10};
    cpp_util::dynarray<int> eve = {1, 2, 3};

    std::cout << std::boolalpha;

    // compare non equal
    std::cout << "alice == bob returns " << (alice == bob) << '\n';
    std::cout << "alice != bob returns " << (alice != bob) << '\n';
    std::cout << "alice <  bob returns " << (alice < bob) << '\n';
    std::cout << "alice <= bob returns " << (alice <= bob) << '\n';
    std::cout << "alice >  bob returns " << (alice > bob) << '\n';
    std::cout << "alice >= bob returns " << (alice >= bob) << '\n';
    // compare equal
    std::cout << "alice == eve returns " << (alice == eve) << '\n';
    std::cout << "alice != eve returns " << (alice != eve) << '\n';
    std::cout << "alice <  eve returns " << (alice < eve) << '\n';
    std::cout << "alice <= eve returns " << (alice <= eve) << '\n';
    std::cout << "alice >  eve returns " << (alice > eve) << '\n';
    std::cout << "alice >= eve returns " << (alice >= eve) << "\n\n";
  }

#if defined(__cpp_constexpr_dynamic_alloc) && defined(__cpp_lib_constexpr_numeric)
  // constexpr test
  std::cout << force_compile_time_evaluation<gaus_sum(10)>() << std::endl;
#endif

  return 0;
}
