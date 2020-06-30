// Let's initialize a class for objects that have multiple
// double values and multiple names
// - Use std::array<> instead of std::vector<>
// - Put multiple Values objects in a unordered_set
#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <cassert>
#include <unordered_set>
#include <algorithm>

namespace ex4 {

template<int MaxSize>
class Values {
 private:
  std::string name;
  std::array<double, MaxSize> ages;
  std::size_t agesSize;
 public:
  Values(std::string inputName,
         std::initializer_list<double> inputAges = {})
   : name{inputName}, ages{}, agesSize{inputAges.size()} {
      assert(inputAges.size() <= MaxSize);
      std::size_t idx = 0;
      for (const auto& age : inputAges) {
        ages[idx] = age;
        ++idx;
      }
  }
  Values(std::initializer_list<double> inputAges)
   : Values{{}, inputAges} {
  }

  void print() const {
    std::cout << "\nValues of name '" << name << "':\n";
    for (std::size_t idx = 0; idx < agesSize; ++idx) {
      std::cout << "    " << ages[idx] << '\n';
    }
  }

  std::string getName() const {
    return name;
  }

  friend bool operator== (const Values<MaxSize>& v1,
                          const Values<MaxSize>& v2) {
    if (v1.name != v2.name) {
      return false;
    }
    if (v1.ages != v2.ages) {
      return false;
    }
    return true;
  }

  // print whenever a Values object is copied/assigned
  // a) without move semantics
  // b) look when move semantics is used
  // a) => implement printing copy constructor/assignment:
  Values(const Values& val)
   : name{val.name}, ages{val.ages}, agesSize{val.agesSize} {
      std::cout << "COPY '" << val.name << "'\n";
  }
  Values& operator=(const Values& val) {
    name = val.name;
    ages = val.ages;
    agesSize = val.agesSize;
    std::cout << "COPYASSIGN '" << name << "'\n";
    return *this;
  }
  // b) implementing also move operations:
  Values(Values&& val)
   : name{std::move(val.name)}, ages{std::move(val.ages)},
     agesSize{val.agesSize} {
      std::cout << "MOVE '" << name << "'\n";
  }
  Values& operator=(Values&& val) {
    name = std::move(val.name);
    ages = std::move(val.ages);
    agesSize = val.agesSize;
    std::cout << "MOVEASSIGN '" << name << "'\n";
    return *this;
  }
};

template<int MaxSize>
struct ValuesHash {
  std::size_t operator() (const Values<MaxSize>& val) const {
    // return the hash value according to the name of the Values object:
    std::hash<std::string> hashFunc;
    return hashFunc(val.getName());
  }
};

void testValuesInUnorderedSet()
{
  std::vector<Values<5>> coll{
    Values<5>{"Jim", {4.2, 3.3, 7.7, 18}},
    Values<5>{"Tim", {64, 64, 65, 66, 67}},
    Values<5>{"Tom", {65, 66, 67}},
    Values<5>{"Alessandro"},
  };
  std::cout << "----------------------------------------------\n";
  coll.push_back(Values<5>{"Paul"});
  std::cout << "----------------------------------------------\n";
  std::sort(coll.begin(), coll.end(),
            [](const auto& elem1, const auto& elem2) {
              return elem1.getName() < elem2.getName();
            });

  std::cout << "================= Values objects: ============\n";
  // print the Values Objects:
  for (const auto& elem : coll) {
    elem.print();
  }
  std::cout << "==============================================\n";
}

}  // namespace ex4

// TEST---------------------------------------------------------------------------------------------------------------|
#include "gtest/gtest.h"

namespace {

using namespace ex4;


TEST(Test, Test1) {
  testValuesInUnorderedSet();
}

}  // namespace
