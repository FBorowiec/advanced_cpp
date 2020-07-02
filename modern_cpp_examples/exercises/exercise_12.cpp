#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace exercise_12 {

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

Team<double> CreateTeamDouble() {
  static int id = 0;

  Team<double> t{"Team" + std::to_string(id)};
  t.Insert2(-id);
  id++;
  return t;
}

template <typename Cont, typename TeamT>
void InsertTwice(Cont& cont, TeamT&& team) {
  cont.push_back(team);  // copy
  // NO! cont.push_back(std::forward<TeamT>(team));  // move ALWAYS
  cont.push_back(std::forward<TeamT>(team));  // move only if passed with move
}

}  // namespace exercise_12

// TEST---------------------------------------------------------------------------------------------------------------|
#include "gtest/gtest.h"

namespace {

using namespace exercise_12;

TEST(CreateTeamTest, MoveTeam) {
  std::vector<Team<double>> coll;
  coll.reserve(
      10);  // otherwise when vector overflows it will copy the team coll
  for (int i{0}; i < 10; ++i) {
    coll.emplace_back(CreateTeamDouble());
  }
  coll[4].Insert2(42);

  Team<double> teamX{"Some name for 2 teams..."};

  InsertTwice(coll, std::move(teamX));
}

}  // namespace
