#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "modern_cpp_examples/map/utils/buckets.hpp"

namespace exercise_5 {

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

  std::vector<T> GetValues() { return values_; }

 private:
  std::vector<std::string> names_;
  std::vector<T> values_;
};

}  // namespace exercise_5

// TEST---------------------------------------------------------------------------------------------------------------|
#include "gtest/gtest.h"

namespace {

using namespace exercise_5;

TEST(TeamTest, TestAssociativeContainer) {
  Team<double> team1{"Jim", "Gianna", "Andrea"};
  team1.Insert2(42, 7, 5.5);
  Team<double> team2{"John", "Jim", "Jack"};
  team2.Insert2(5, 7, 6);
  Team<double> team3{"Marc", "Andy", "Dean"};
  team3.Insert2(1, 2, 3);

  std::unordered_map<std::string, Team<double>> teams{{"Team 1", team1},
                                                      {"Team 2", team2}};

  // array should never be so small that it's factor of 10 of elements - wasting a lot of memory
  teams.max_load_factor(0.7f);

  teams.insert({"Team 3", team3});

  for (int i{0}; i<1000; ++i) {
    Team<double> t{"Unknown"};
    t.Insert2(i, i*100, i*1000, -i);
    teams.insert({"Team " + std::to_string(i), t});
  }

  std::cout << "All teams: \n";
  for (const auto& team : teams) {
    std::cout << " " << team.first << ": " << team.second << std::endl;
  }

  printHashTableState(teams);
}

}  // namespace
