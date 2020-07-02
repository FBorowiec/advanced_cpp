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
template <typename T>
class Team {
 public:
  Team(const std::initializer_list<std::string>& names) : names_(names) {
    assert(names.size() > 1 || (names.size() == 1 && !names.begin()->empty()));
  }
  Team() = delete;

  /**
   * All of the copy/move constructors/assignments are created for you!
   */

  Team(const Team& other)
      : names_(other.names_), values_(other.values_) {  // cpy cntr
    std::cout << "Copy: " << *this << std::endl;
  }
  Team& operator=(const Team& other) {  // cpy assignment
    names_ = other.names_;
    values_ = other.values_;
    std::cout << "Copy assignment: " << *this << std::endl;

    return *this;
  }

  Team(Team&& other)
      : names_(std::move(other.names_)),
        values_(std::move(other.values_)) {  // move cntr
    std::cout << "Move: " << *this << std::endl;
  }
  Team& operator=(Team&& other) {  // move assignment
    names_ = std::move(other.names_);
    values_ = std::move(other.values_);
    std::cout << "Move assignment: " << *this << std::endl;

    return *this;
  }

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

  const std::vector<T>& GetValues() const { return values_; }

 private:
  std::vector<std::string> names_;
  std::vector<T> values_;
};

Team<double> CreateTeamDouble() {
  static int id = 0;

  Team<double> t{"Team" + std::to_string(id)};
  t.Insert2(-id);
  id++;
  return t;
}

}  // namespace exercise_10

// TEST---------------------------------------------------------------------------------------------------------------|
#include "gtest/gtest.h"

namespace {

using namespace exercise_10;

TEST(CreateTeamTest, MoveTeam) {
  std::vector<Team<double>> coll;
  coll.reserve(
      10);  // otherwise when vector overflows it will copy the team coll
  for (int i{0}; i < 10; ++i) {
    coll.emplace_back(CreateTeamDouble());
  }
  coll[4].Insert2(42);

  std::cout << "Sort:\n";
  std::sort(coll.begin(), coll.end(), [](const auto& t1, const auto& t2) {
    return t1.GetValues().size() < t2.GetValues().size();
  });

  for (const auto& elem : coll) {
    std::cout << " " << elem << std::endl;
  }
}

}  // namespace
