#include <algorithm>
#include <array>
#include <cassert>
#include <deque>
#include <iostream>
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace exercise_4 {

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

  std::for_each(coll.begin(), std::next(coll.begin(), 2),
                [&](auto& elem) { std::cout << " " << elem << std::endl; });

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

  std::vector<T> GetValues() { return values_; }

 private:
  std::vector<std::string> names_;
  std::vector<T> values_;
};

}  // namespace exercise_4

// TEST---------------------------------------------------------------------------------------------------------------|
#include "gtest/gtest.h"

namespace {

using namespace exercise_4;

TEST(TeamTest, CreationOfTeams) {
  Team<double> teamA{"Jim", "Gianna", "Andrea"};
  // teamA.Insert1(42, 7, 5.5);
  // teamA.Insert1(42.7);
  // teamA.Insert();  // WRONG: deleted function!
  std::cout << teamA << std::endl;

  Team<std::string> teamB{"Jim", "Gianna", "Andrea"};
  teamB.Insert2("42", "7", "5.5");
  teamB.Insert2("another interesting values");
  std::cout << teamB << std::endl;
}

TEST(PrintCollectionTest, PrintingIntVectorContainer) {
  std::cout << "---vector<int>---" << std::endl;
  PrintCollection(std::vector<int>{1, 2, 3, 4, 5});
  PrintCollection(std::vector<int>{1, 2, 3, 4});
  PrintCollection(std::vector<int>{1, 2, 3});
  PrintCollection(std::vector<int>{1, 2});
  PrintCollection(std::vector<int>{1});
  PrintCollection(std::vector<int>{});
}

TEST(PrintCollectionTest, PrintingStringVectorContainer) {
  std::cout << "---vector<string>---" << std::endl;
  PrintCollection(std::vector<std::string>{"1", "2", "3", "4", "5"});
}

TEST(PrintCollectionTest, PrintingStringSetContainer) {
  std::cout << "---set<string>---" << std::endl;
  PrintCollection(std::set<std::string>{"1", "2", "3", "4", "5"});
}

TEST(PrintCollectionTest, PrintingDoubleUnorderedsetContainer) {
  std::cout << "---unordered_set<double>---" << std::endl;
  PrintCollection(std::unordered_set<double>{1.0, 2.0, 3.0, 4.0, 5.0});
}

TEST(PrintCollectionTest, PrintingCharDequeContainer) {
  std::cout << "---deque<char>---" << std::endl;
  PrintCollection(std::deque<char>{'a', 'b', 'c', 'd', 'e'});
}

TEST(PrintCollectionTest, PrintingStringInitializerListContainer) {
  std::cout << "---initializer_list<string>---" << std::endl;
  PrintCollection(
      std::initializer_list<std::string>{"abc", "def", "ghi", "jkl", "mno"});
}

TEST(PrintCollectionTest, PrintingLongListContainer) {
  std::cout << "---list<long>---" << std::endl;
  PrintCollection(std::list<long>{1, 2, 3, 4, 5});
}

TEST(PrintCollectionTest, PrintingUnsignedMultisetContainer) {
  std::cout << "---multiset<unsigned>---" << std::endl;
  PrintCollection(std::multiset<unsigned>{1, 2, 3, 4, 5});
}

TEST(PrintCollectionTest, PrintingTeamContainer) {
  std::cout << "---team.value => vector<string>---" << std::endl;
  Team<std::string> team{"Jim", "Gianna", "Andrea"};
  team.Insert2("42", "7", "5.5");
  PrintCollection(team.GetValues());
}

TEST(PrintCollectionTest, PrintingFloatArrayContainer) {
  std::cout << "---array<float>---" << std::endl;
  PrintCollection(
      std::array<float, 5>{1.0f, 2.0f, 3.0f, 4.0f, 5.0f});  // doesn't work
}

}  // namespace
