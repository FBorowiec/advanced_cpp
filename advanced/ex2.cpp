// Let's initialize a class for objects that have multiple
// double values and multiple names
#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <cassert>

namespace ex2 {

template<int MaxSize>
class Values {
 private:
  std::array<std::string, MaxSize> names;
  std::size_t namesSize;
  std::array<double, MaxSize> ages;
  std::size_t agesSize;
 public:
  Values(std::initializer_list<std::string> inputNames,
         std::initializer_list<double> inputAges = {})
   : names{}, namesSize{inputNames.size()},
     ages{}, agesSize{inputAges.size()} {
      assert(inputNames.size() <= MaxSize && inputAges.size() <= MaxSize);
      int idx = 0;
      for (const auto& name : inputNames) {
        names[idx] = name;
        ++idx;
      }
      idx = 0;
      for (const auto& age : inputAges) {
        ages[idx] = age;
        ++idx;
      }
  }
  Values(std::initializer_list<double> inputAges)
   : Values{{}, inputAges} {
  }

  void print() {
    std::cout << "\nValues:\n";
    std::cout << "- names: \n";
    for (std::size_t idx = 0; idx < namesSize; ++idx) {
      std::cout << "    " << names[idx] << '\n';
    }
    std::cout << "- ages: \n";
    for (std::size_t idx = 0; idx < agesSize; ++idx) {
      std::cout << "    " << ages[idx] << '\n';
    }
  }
};

}  // namespace ex2

// TEST---------------------------------------------------------------------------------------------------------------|
#include "gtest/gtest.h"

namespace {

using namespace ex2;

TEST(Test, Test1) {
  Values<5> v1{{"Jim", "Lisa", "John"}, {4.2, 3.3, 7.7, 18}};
  v1.print();
  Values<2> v2{"Jim", "Tom"}; // same as: Values v2{{"Jim", "Tom"}};
  v2.print();
  Values<4> v3{0, 8, 15, 7734.5557};
  v3.print();
}

}  // namespace
