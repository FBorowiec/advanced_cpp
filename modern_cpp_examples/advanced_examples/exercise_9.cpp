#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace exercise_9 {

template <typename T>
class Team {
 public:
  Team(const std::initializer_list<std::string>& names) : names_(names) {
    assert(names.size() > 1 || (names.size() == 1 && !names.begin()->empty()));
  }
  Team() = delete;
  Team(const Team& other)
      : names_(other.names_), values_(other.values_) {  // cpy cntr
    std::cout << "Copy: " << *this << std::endl;
  }
  Team& operator=(const Team& other) {  // cpy assignment
    names_ = other.names_;
    values_ = other.values_;

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

int countMoreThan(const std::unordered_map<std::string, Team<double>>& teams,
                  int max) {
  return std::count_if(teams.begin(), teams.end(), [max](const auto& team) {
    return std::accumulate(team.second.GetValues().begin(),
                           team.second.GetValues().end(), 0.0) > max;
  });
}

void PrintAllTeams(const std::unordered_map<std::string, Team<double>>& teams) {
  std::cout << "All teams: \n";
  for (const auto& team : teams) {
    std::cout << " " << team.first << ": " << team.second << std::endl;
  }
}

}  // namespace exercise_9

// TEST---------------------------------------------------------------------------------------------------------------|
#include "gtest/gtest.h"

namespace {

using namespace exercise_9;

TEST(MoveSemantics, Test1) {
  Team<double> team1{"Jim", "Gianna", "Andrea"};
  team1.Insert2(142, 17, 5.5);
  Team<double> team2{"John", "Jim", "Jack"};
  team2.Insert2(15, 7, 6);
  Team<double> team3{"Marc", "Andy", "Dean"};
  team3.Insert2(1, 2, 103);

  std::unordered_map<std::string, Team<double>> teams{
      std::pair<std::string, Team<double>>{"Team 1", team1},
      std::pair<std::string, Team<double>>{"Team 2", team2},
      std::pair<std::string, Team<double>>{"Team 3", team3}};

  Team<double> team100{"Jim", "Tina"};
  team100.Insert2(50, 50.1);
  teams.insert({"Team 100", team100});
}

}  // namespace
