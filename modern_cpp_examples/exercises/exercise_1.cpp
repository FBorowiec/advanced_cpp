#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace exercise_1 {

class Team {
 public:
  Team(const std::initializer_list<std::string>& names) : names_(names) {
    assert(names.size() > 1 || (names.size() == 1 && !names.begin()->empty()));
  }
  Team() = delete;

  void Insert(const std::initializer_list<double>& values) {
    values_.insert(values_.end(), values);
    // for (const auto& value : values) {
    //   values_.push_back(value);
    // }
  }

  friend std::ostream& operator<<(std::ostream& os, const Team& t) {
    os << "[ ";
    for (const auto& name : t.names_) {
      os << name << ' ';
    }
    os << ": ";
    for (const auto& value : t.values_) {
      os << value << ' ';
    }
    os << "]";
    return os;
  }

 private:
  std::vector<std::string> names_;
  std::vector<double> values_;
};

}  // namespace exercise_1

// TEST---------------------------------------------------------------------------------------------------------------|
#include "gtest/gtest.h"

namespace {

using namespace exercise_1;

TEST(Test, Test1) {
  Team teamA({"Jim", "Gianna", "Andrea"});
  teamA.Insert({42, 7, 5.5});
  teamA.Insert({42.7});
  std::cout << teamA << std::endl;

  // Team teamB();  // WRONG: This is a function declaration!
  // std::cout << teamB << std::endl;

  // Team teamC;  // WRONG: No default constructor!
  // std::cout << teamC << std::endl;

  // Team teamD{};  // WRONG: Passing no value deleted!
  // std::cout << teamD << std::endl;

  // Team teamE{{}};  // WRONG: Assertion fails
  // std::cout << teamE << std::endl;
}

}  // namespace
