#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace exercise_10 {

class Customer {
 public:
  Customer(const std::string& f, const std::string& l, int i)
      : first_(std::move(f)), last_(std::move(l)), id_(i) {}

 private:
  std::string first_;
  std::string last_;
  int id_;
};

}  // namespace exercise_10

// TEST---------------------------------------------------------------------------------------------------------------|
#include "gtest/gtest.h"

namespace {

using namespace exercise_10;

TEST(TrivialClassTest, MoveSemantics) {
  std::string s = "Joe";
  Customer c{"Joe", "Fix", 42};         // at least 2 mallocs
  Customer d{s, "Fix", 42};             // at least 2 mallocs
  Customer e{std::move(s), "Fix", 42};  // at least 1 malloc
}

}  // namespace
