// Let's initialize a class for objects that have multiple
// double values and multiple names
#include <vector>
#include <string>
#include <iostream>

namespace ex1 {

class Values {
 private:
  std::vector<std::string> names;
  std::vector<double> ages;
 public:
  Values(std::initializer_list<std::string> inputNames,
         std::initializer_list<double> inputAges = {})
   : names{inputNames}, ages{inputAges} {
  }
  Values(std::initializer_list<double> inputAges)
   : Values{{}, inputAges} {
  }

  void print() {
    std::cout << "\nValues:\n";
    std::cout << "- names: \n";
    for (const auto& name : names) {
      std::cout << "    " << name << '\n';
    }
    std::cout << "- ages: \n";
    for (const auto& age : ages) {
      std::cout << "    " << age << '\n';
    }
  }
};

}  // namespace

// TEST---------------------------------------------------------------------------------------------------------------|
#include "gtest/gtest.h"

namespace {

using namespace ex1;

TEST(Test, Test1) {
  Values v1{{"Jim", "Lisa", "John"}, {4.2, 3.3, 7.7, 18}};
  v1.print();
  Values v2{"Jim", "Tom"}; // same as: Values v2{{"Jim", "Tom"}};
  v2.print();
  Values v3{0, 8, 15, 7734.5557};
  v3.print();
}

}  // namespace
