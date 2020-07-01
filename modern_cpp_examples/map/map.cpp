#include <iostream>
#include <string>
#include <unordered_map>

#include "modern_cpp_examples/map/utils/buckets.hpp"

namespace map {

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------

int main() {
  using namespace std;

  // compute hash value:
  std::hash<std::string> hashFunc;
  std::cout << "strange => " << hashFunc("strange") << '\n';
  std::cout << "hour    => " << hashFunc("hour") << '\n';
  std::cout << "day     => " << hashFunc("day") << '\n';
  std::cout << "trait   => " << hashFunc("trait")
            << " %11: " << hashFunc("trait") % 11 << '\n';
  std::cout << "car     => " << hashFunc("car")
            << " %11: " << hashFunc("car") % 11 << '\n';

  // create and initialize multimap as string/string dictionary
  unordered_multimap<string, string> dict{
      {"day", "Tag"},          {"strange", "fremd"}, {"car", "Auto"},
      {"smart", "elegant"},    {"trait", "Merkmal"}, {"strange", "seltsam"},
      {"smart", "raffiniert"}, {"smart", "klug"},
  };
  std::cout << "array size: " << dict.bucket_count() << '\n';
  printHashTableState(dict);
  std::cout << "----\n";

  dict.emplace("hour", "Stunde");
  dict.emplace("wine", "Wein");
  dict.emplace("beer", "Bier");
  dict.emplace("party", "Party");
  printHashTableState(dict);
  std::cout << "----\n";

  // print all elements
  for (const auto& elem : dict) {
    cout << elem.first << " <-> " << elem.second << '\n';
  }

  // find specific element
  auto pos = dict.find("smart");
  if (pos != dict.end()) {
    cout << "smart: " << pos->second << '\n';
    //...
  }

  return 0;
}

}  // namespace map

// TEST---------------------------------------------------------------------------------------------------------------|
#include "gtest/gtest.h"

namespace {

using namespace map;

TEST(Test, Test1) { main(); }

}  // namespace
