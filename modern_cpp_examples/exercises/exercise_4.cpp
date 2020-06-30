#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace exercise_4 {

/**
 * Using std::begin and std::end allows iterating over:
 * std::array, std::vector, std::map, std::unordered_map, std::map,
 * std::unordered_set, std::set
 */
template <template <typename> typename Container, typename Type>
void PrintCollection(const Container<Type>& coll) {
  std::cout << "Coll: " << std::endl;

  if (std::begin(coll) == std::end(coll)) {
    std::cout << " empty" << std::endl;
    return;
  }

  std::for_each(coll.begin(), coll.end(),
                [&](Type elem) { std::cout << " " << elem << std::endl; });
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
  // teamA.Insert1(42, 7, 5.5);  // TODO: Why not working?
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
  PrintCollection<std::vector, int>({1, 2, 3, 4, 5});
  PrintCollection<std::vector, int>({1, 2, 3, 4});
  PrintCollection<std::vector, int>({1, 2, 3});
  PrintCollection<std::vector, int>({1, 2});
  PrintCollection<std::vector, int>({1});
  PrintCollection<std::vector, int>({});
}

TEST(PrintCollectionTest, PrintingStringVectorContainer) {
  std::cout << "---vector<string>---" << std::endl;
  PrintCollection<std::vector, std::string>({"1", "2", "3", "4", "5"});
}

TEST(PrintCollectionTest, PrintingStringSetContainer) {
  std::cout << "---set<string>---" << std::endl;
  PrintCollection<std::set, std::string>({"1", "2", "3", "4", "5"});
}

TEST(PrintCollectionTest, PrintingDoubleUnorderedsetContainer) {
  std::cout << "---unordered_set<double>---" << std::endl;
  PrintCollection<std::unordered_set, double>({1.0, 2.0, 3.0, 4.0, 5.0});
}

// TEST(PrintCollectionTest, PrintingFloatArrayContainer) {
// std::cout << "---unordered_set<double>---" << std::endl;
// std::array<float, 5> arr{1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
// PrintCollection(arr);
// PrintCollection<std::array, float, 5>({1.0f, 2.0f, 3.0f, 4.0f, 5.0f});  //
// doesn't work PrintCollection<float[5]>({1.0f, 2.0f, 3.0f, 4.0f, 5.0f});  //
// doesn't work PrintCollection<std::array<float,
// 5>>({1.0f, 2.0f, 3.0f, 4.0f, 5.0f});  // doesn't work
// PrintCollection<std::array<float, 5>, float>({1.0f, 2.0f, 3.0f, 4.0f, 5.0f});
// // doesn't work PrintCollection<std::array,
// float>({1.0f, 2.0f, 3.0f, 4.0f, 5.0f});  // doesn't work
// }

// TEST(PrintCollectionTest, PrintingConstCharPtrUnorderedMapContainer) {
//   std::cout << "---unordered_set<double>---" << std::endl;
//   PrintCollection<std::unordered_map, const char*>({'1', '2', '3', '4',
//   '5'});
// }

}  // namespace
