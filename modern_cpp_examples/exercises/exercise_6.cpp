#include <algorithm>
#include <array>
#include <cassert>
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "modern_cpp_examples/map/utils/buckets.hpp"

namespace exercise_6 {

template <typename T>
void PrintCollection(const T& coll) {
  std::cout << "Coll: " << std::endl;

  int i{0};
  for (auto pos = coll.begin(); pos != coll.end() && i < 2; ++pos) {
    std::cout << " " << *pos << std::endl;
    ++i;
  }

  if (coll.size() >= 4) {
    std::cout << " ..." << std::endl;
  }

  if (coll.size() > 2) {
    std::cout << " " << coll.back() << std::endl;
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

}  // namespace exercise_6

// TEST---------------------------------------------------------------------------------------------------------------|
#include "gtest/gtest.h"

namespace {

using namespace exercise_6;

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

TEST(PrintCollectionTest, PrintingCharDequeContainer) {
  std::cout << "---deque<char>---" << std::endl;
  PrintCollection(std::deque<char>{'a', 'b', 'c', 'd', 'e'});
}

TEST(PrintCollectionTest, PrintingLongListContainer) {
  std::cout << "---list<long>---" << std::endl;
  PrintCollection(std::list<long>{1, 2, 3, 4, 5});
}

}  // namespace
