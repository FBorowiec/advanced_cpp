// Let's initialize a class for objects that have multiple
// double values and multiple names
// - Use std::array<> instead of std::vector<>
// - Put multiple Values objects in a unordered_set
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

namespace ex3 {

template <int MaxSize>
class Values {
 private:
  std::string name;
  std::array<double, MaxSize> ages;
  std::size_t agesSize;

 public:
  Values(std::string inputName, std::initializer_list<double> inputAges = {})
      : name{inputName}, ages{}, agesSize{inputAges.size()} {
    assert(inputAges.size() <= MaxSize);
    std::size_t idx = 0;
    for (const auto& age : inputAges) {
      ages[idx] = age;
      ++idx;
    }
  }
  Values(std::initializer_list<double> inputAges) : Values{{}, inputAges} {}

  void print() const {
    std::cout << "\nValues of name '" << name << "':\n";
    for (std::size_t idx = 0; idx < agesSize; ++idx) {
      std::cout << "    " << ages[idx] << '\n';
    }
  }

  std::string getName() const { return name; }

  friend bool operator==(const Values<MaxSize>& v1, const Values<MaxSize>& v2) {
    if (v1.name != v2.name) {
      return false;
    }
    if (v1.ages != v2.ages) {
      return false;
    }
    return true;
  }
};

template <int MaxSize>
struct ValuesHash {
  std::size_t operator()(const Values<MaxSize>& val) const {
    // return the hash value according to the name of the Values object:
    std::hash<std::string> hashFunc;
    return hashFunc(val.getName());
  }
};

void testValuesInUnorderedSet() {
  std::unordered_set<Values<5>, ValuesHash<5>> coll{
      Values<5>{"Jim", {4.2, 3.3, 7.7, 18}},
      Values<5>{"Tim", {64, 64, 65, 66, 67}},
      Values<5>{"Tom", {65, 66, 67}},
      Values<5>{"Alessandro"},
  };

  std::cout << "================= Values objects: ============\n";
  // print the Values Objects:
  for (const auto& elem : coll) {
    elem.print();
  }
  // print out using count_if() how many names of Values objects
  // have 0-3 characters:
  int num = std::count_if(coll.begin(), coll.end(), [](const auto& elem) {
    return elem.getName().size() <= 3;
  });
  std::cout << "elems with short names: " << num << '\n';
  std::cout << "==============================================\n";
}

void testValues() {
  Values<5> v1{"Jim", {4.2, 3.3, 7.7, 18}};
  v1.print();
  Values<2> v2{"Jim"};
  v2.print();
  Values<4> v3{0, 8, 15, 7734.5557};
  v3.print();
}

}  // namespace ex3

// TEST---------------------------------------------------------------------------------------------------------------|
#include "gtest/gtest.h"

namespace {

using namespace ex3;

TEST(Test, Test1) {
  testValuesInUnorderedSet();
  testValues();
}

}  // namespace
