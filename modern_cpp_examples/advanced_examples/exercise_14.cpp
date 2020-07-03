#include <algorithm>
#include <cassert>
#include <chrono>
#include <future>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace exercise_14 {

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
  cont.push_back(team);                       // copy
  cont.push_back(std::forward<TeamT>(team));  // move only if passed with move
}

template <typename Cont, typename TeamT, typename... TeamTargs>
void InsertTwice(Cont& cont, TeamT first, TeamTargs&&... teams) {
  cont.push_back(first);
  cont.push_back(std::forward<TeamT>(first));
  InsertTwice(cont, teams...);
}

template <typename T>
void PrintCollection(std::mutex& coutMutex, const T& coll) {
  for (const auto& elem : coll) {
    std::lock_guard<std::mutex> lg{coutMutex};
    std::cout << " " << elem << '\n';
  }
}

}  // namespace exercise_14

// TEST---------------------------------------------------------------------------------------------------------------|
#include "gtest/gtest.h"

namespace {

using namespace exercise_14;

TEST(CreateTeamTest, MutexTest) {
  std::vector<Team<double>> coll;

  // allow to insert an artibrary number of teams with an without move sematics
  Team<double> teamA{"Team A"};
  Team<double> teamB{"Team B"};
  Team<double> teamC{"Team C"};
  Team<double> teamX{"Some name for 2 teams..."};

  InsertTwice(coll, teamA, teamB, std::move(teamC), std::move(teamX));

  // Print collection in parallel
  std::mutex coutMutex;
  std::future<void> print1 =
      std::async(std::launch::async, PrintCollection<decltype(coll)>,
                 std::ref(coutMutex), std::cref(coll));
  std::future<void> print2 =
      std::async(std::launch::async, PrintCollection<decltype(coll)>,
                 std::ref(coutMutex), std::cref(coll));

  print1.get();
  print2.get();
}

}  // namespace
