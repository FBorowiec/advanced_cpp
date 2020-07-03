#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <future>
#include <map>
#include <numeric>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace exercise_12 {

template <typename T>
void PrintCollection(const T& coll) {
  std::cout << "Coll: " << std::endl;

  if (std::size(coll) == 0) {
    std::cout << " empty" << std::endl;
    return;
  }

  if (std::size(coll) == 1) {
    std::cout << " " << *(std::prev(coll.end(), 1)) << std::endl;
    return;
  }

  std::for_each(coll.begin(), std::next(coll.begin(), 2), [](const auto& elem) {
    std::cout << " " << elem << std::endl;
  });

  if (std::size(coll) >= 3) {
    if (std::size(coll) > 3) {
      std::cout << " ..." << std::endl;
    }
    std::cout << " " << *(std::next(coll.begin(), std::size(coll) - 1))
              << std::endl;
  }
}

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

}  // namespace exercise_12

// TEST---------------------------------------------------------------------------------------------------------------|
#include "gtest/gtest.h"

namespace {

using namespace exercise_12;

TEST(CreateTeamTest, InsertTwiceTest) {
  std::vector<Team<double>> coll;

  // allow to insert an artibrary number of teams with an without move sematics
  Team<double> teamA{"Team A"};
  Team<double> teamB{"Team B"};
  Team<double> teamC{"Team C"};
  Team<double> teamX{"Some name for 2 teams..."};

  InsertTwice(coll, std::move(teamX));

  InsertTwice(coll, teamA, teamB, std::move(teamC));

  for (const auto& el : coll) {
    std::cout << el << "\n";
  }
  std::cout << "\n";
}

TEST(CreateTeamTest, MoveTeam) {
  std::vector<Team<double>> coll;
  coll.reserve(
      10);  // otherwise when vector overflows it will copy the team coll
  for (int i{0}; i < 10; ++i) {
    coll.emplace_back(CreateTeamDouble());
  }
  coll[4].Insert2(42);

  // allow to insert an artibrary number of teams with an without move sematics
  Team<double> teamA{"Team A"};
  Team<double> teamB{"Team B"};
  Team<double> teamC{"Team C"};
  Team<double> teamX{"Some name for 2 teams..."};

  InsertTwice(coll, std::move(teamX));

  InsertTwice(coll, teamA, teamB, std::move(teamC));

  // Print collection in parallel
  std::future<void> print = std::async(std::launch::async, PrintCollection<decltype(coll)>, std::cref(coll));

  // sum with two lambdas
  auto sum_values1 = std::async(std::launch::async, [&coll] {
    return std::accumulate(coll.begin(), coll.end(), 0.0, [] (double current_sum, const auto& team) {
      auto values = team.GetValues();
      return std::accumulate(values.begin(), values.end(), current_sum);
    });
  });

  // sum with two for loops
  std::future<double> sum_values2 = std::async(std::launch::async, [&coll] {
    double total_sum{0};
    for (const auto& team : coll) {
      for (const auto& value : team.GetValues()) {
        total_sum += value;
      }
    }
    return total_sum;
  });
  print.get();
  std::cout << "Total sum: " << sum_values1.get() << std::endl;
  std::cout << "Total sum: " << sum_values2.get() << std::endl;
}

}  // namespace
