#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace exercise_3 {

template <typename T>
class Team {
 public:
  Team(const std::initializer_list<std::string>& names) : names_(names) {
    assert(names.size() > 1 || (names.size() == 1 && !names.begin()->empty()));
  }
  Team() = delete;

  // Option 1: recursively
  template <typename U, typename... Args>
  void Insert1(U element, Args... values) {
    values_.push_back(element);
    Insert1(values...);
  }

  // Option 2: no compile-time recursion
  template <typename... Args>
  void Insert2(Args... values) {
    values_.insert(values_.end(), {static_cast<T>(values)...});
  }

  void Insert1() = delete;
  void Insert2() = delete;

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
  std::vector<T> values_;
};

}  // namespace exercise_3

// TEST---------------------------------------------------------------------------------------------------------------|
#include "gtest/gtest.h"

namespace {

using namespace exercise_3;

TEST(TeamTest, CreationOfTeams) {
  Team<double> teamA{"Jim", "Gianna", "Andrea"};
  // teamA.Insert1(42, 7, 5.5);  // TODO: Why not working?
  // teamA.Insert1(42.7);
  // teamA.Insert();  // WRONG: deleted function!
  std::cout << teamA << std::endl;

  Team<std::string> teamB{"Jim", "Gianna", "Andrea"};
  teamB.Insert2("42", "7", "5.5");
  teamB.Insert2("another interesting values");
  std::cout << teamB << std::endl;
}

}  // namespace
