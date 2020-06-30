#ifndef PRINT_HPP
#define PRINT_HPP
#include <vector>
#include "customer.hpp"

// print first and last 5 elements of a collection:
template<typename ContainerT>
void printColl (const ContainerT& coll)
{
#ifdef UNDERSTAND_WHAT_IS_PASSED
  // understand what is passed as T:
  std::cout << std::boolalpha;
  std::cout << "coll is:\n";
  std::cout << "  const char[4]: "
            << std::is_same<decltype(coll), const char[4]>::value
            << '\n';
  std::cout << "  const char*: "
            << std::is_same<decltype(coll), const char*>::value
            << '\n';
  std::cout << "  const char(&)[4]: "
            << std::is_same<decltype(coll), const char(&)[4]>::value
            << '\n';
  std::cout << "ContainerT is:\n";
  std::cout << "  char[4]: "
            << std::is_same<ContainerT, char[4]>::value
            << '\n';
  std::cout << "  const char[4]: "
            << std::is_same<ContainerT, const char[4]>::value
            << '\n';
  std::cout << "  const char*: "
            << std::is_same<ContainerT, const char*>::value
            << '\n';
  for (const auto& elem : coll) {
    std::cout << elem << '\n';
  }
#endif

    //typename ContainerT::const_iterator pos;
    std::size_t num = 0;
    unsigned dist = std::distance(std::begin(coll), std::end(coll));
    for (auto pos = std::cbegin(coll); pos != std::cend(coll); ++pos, ++num) {
      if (num < 5 || num > dist - 6) {
        std::cout << *pos << '\n';
      }
      if (num == 5 && dist > 10) {
        std::cout << "..." << '\n';
      }
    }
}

#endif // PRINT_HPP
