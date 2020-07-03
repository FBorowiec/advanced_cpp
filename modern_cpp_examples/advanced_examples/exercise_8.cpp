#include <algorithm>
#include <array>
#include <cassert>
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "modern_cpp_examples/map/utils/buckets.hpp"

namespace exercise_8 {

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

}  // namespace exercise_8

// TEST---------------------------------------------------------------------------------------------------------------|
#include "gtest/gtest.h"

namespace {

using namespace exercise_8;

TEST(CountMoreThan, Test1) {
  Team<double> team1{"Jim", "Gianna", "Andrea"};
  team1.Insert2(142, 17, 5.5);
  Team<double> team2{"John", "Jim", "Jack"};
  team2.Insert2(15, 7, 6);
  Team<double> team3{"Marc", "Andy", "Dean"};
  team3.Insert2(1, 2, 103);

  std::unordered_map<std::string, Team<double>> teams{
      {"Team 1", team1}, {"Team 2", team2}, {"Team 3", team3}};

  Team<double> team100{"Jim", "Tina"};
  team100.Insert2(50, 50.1);
  teams.insert({"Team 100", team100});

  std::hash<std::string> hf;
  std::cout << "Hash value of Team A: " << hf("Team A") << std::endl;
  std::cout << "Hash value of Team B: " << hf("Team A") << std::endl;
  std::cout << "Hash value of Team C: " << hf("Team A") << std::endl;
  std::cout << "Hash value of Team 100: " << hf("Team 100") << std::endl;

  struct StringHasher {
    std::size_t operator()(const std::string& s) {
      return s.size();  // very stupid! all teams with same size will be at same
                        // location!
    }
  };
  std::unordered_map<std::string, Team<double>, StringHasher> teams2;
  teams.insert({"Team 1", team1});
  teams.insert({"Team 2", team2});
  teams.insert({"Team 3", team3});
  teams.insert({"Team 100", team100});
  printHashTableState(teams);
}

}  // namespace
