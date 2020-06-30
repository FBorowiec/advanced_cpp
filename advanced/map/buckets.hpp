#ifndef BUCKETS_HPP
#define BUCKETS_HPP

#include <iostream>
#include <iomanip>
#include <utility>
#include <iterator>
#include <typeinfo>

// generic output for pairs (map elements)
template <typename T1, typename T2>
std::ostream& operator << (std::ostream& strm, const std::pair<T1,T2>& p)
{
    return strm << "[" << p.first << "," << p.second << "]";
}

template <typename T>
void printHashTableState (const T& cont)
{
   using namespace std;

   // basic data:
   cout << "size:                " << cont.size() << '\n';
   cout << "buckets:             " << cont.bucket_count() << '\n';
   cout << "current load factor: " << cont.load_factor() << '\n';
   cout << "max load factor:     " << cont.max_load_factor() << '\n';
   cout << "chaining style:      "
        << (typeid(typename iterator_traits<typename T::iterator>::iterator_category)
              == typeid(bidirectional_iterator_tag)
           ? "doubly-linked"
           : "singly-linked")
        << '\n';

   // elements per bucket:
   cout << "data: " << '\n';    
   for (auto idx=0u; idx != cont.bucket_count(); ++idx) {
      cout << " b[" << std::setw(2) << idx << "]: ";
      for (auto pos=cont.begin(idx); pos != cont.end(idx); ++pos) {
         cout << *pos << " ";
      }
      cout << '\n';
   }
}

#endif // BUCKETS_HPP
