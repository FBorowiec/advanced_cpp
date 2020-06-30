#define _CRT_SECURE_NO_WARNINGS  // avoid VC++ warnings when using sprintf()
#include "customer.hpp"

#include <algorithm>  // for sort()
#include <cassert>
#include <cstdio>   // for sprintf()
#include <cstdlib>  // for atoi()
#include <iostream>
#include <list>
#include <numeric>  // for accumulate()
#include <string>
#include <unordered_map>
#include <vector>

#include "buckets.hpp"
#include "print.hpp"
#include "timer.hpp"

/********************************************
 * generic helpers for main()
 ********************************************/

// add an element twice to a collection:
// ideally:
//   {
//     addTwice(coll, Customer{"test"});  // move temporary
//     Customer c{"test2"};
//     addTwice(coll, c);                 // copy lvalue
//     addTwice(coll, std::move(c));      // move lvalue
//     ...
//   } // end of lifetime of c
#ifdef EITHER_THIS
// classic approach (by value and move):
void addTwice(std::vector<Customer>& coll, const Customer& arg) {
  coll.push_back(arg);  // copies
  coll.push_back(arg);  // copies
}
#elif OR_THIS
// overload for lvalues and rvalues:
void addTwice(std::vector<Customer>& coll, const Customer& arg) {
  coll.push_back(arg);  // copies
  coll.push_back(arg);  // copies
}
void addTwice(std::vector<Customer>& coll, Customer&& arg) {
  coll.push_back(arg);             // copies
  coll.push_back(std::move(arg));  // moves
}
#else
// by value and move:
void addTwice(std::vector<Customer>& coll, Customer arg) {
  coll.push_back(arg);             // copies
  coll.push_back(std::move(arg));  // moves
}
#endif

// sort criterion:
bool lessCustomerValues(const Customer& c1, const Customer& c2) {
  return c1.getSum() < c2.getSum();
}

/********************************************
 * test helpers
 ********************************************/

void testMoveAPerson() {
  std::string s;
  std::cout << "default string capacity: " << s.capacity() << '\n';
  Person p1{"Gasparet"};
  Person p2{std::move(p1)};
  // std::cout << "p1: " << p1 << '\n';
  // std::cout << "p2: " << p2 << '\n';
  Person p3{"Wolfgang Amadeus Mozart"};
  Person p4{std::move(p3)};
  // std::cout << "p3: " << p3 << '\n';
  // std::cout << "p4: " << p4 << '\n';
  assert(p1.getName().empty());
  assert(p2.getName() == "Gasparet");
  assert(p3.getName().empty());
}

void testPrint() {
  std::cout << "----\n";
  std::vector<int> coll{1, 2, 3, 33, 1, 4, 3, 77, 66, 4, -3, -2, -1};
  printColl(coll);

  std::cout << "----\n";
  std::list<int> coll2{1, 2, 3, 42};
  printColl(coll2);

  std::cout << "----\n";
  printColl(std::string{"hey"});
  std::cout << "----\n";
  printColl("hey");
}

namespace customer {

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------

/********************************************
 * main()
 ********************************************/

Customer createCustomer() {
  Customer c{"test customer"};
  c.addValue(42);
  c.addValue(42);
  c.addValue(42);
  return c;
}

int main(int argc, const char** argv) {
  testMoveAPerson();
  testPrint();
  // return 0;

  Timer timer("main()");
  // initialize number of customers (default: 1000):
  int numElems = 1000;
  if (argc > 1) {
    numElems = std::atoi(argv[1]);
  }
  char msg[100];
  std::sprintf(msg, "init %d customers", numElems + 9);
  timer.check(msg);

  // initialize some customers:
  std::vector<Customer> customers;
  // customers.reserve(numElems+20);
  customers = {
      {"Phil Jenkins", Country::usa},
      {"Lisa Brown", Country::uk},
      {"Tim Winter", Country::usa},
      {"C++ Consult"},
      {"Max Meyer", Country::germany},
      {"Ernst Klein", Country::germany},
      {"IT Corp"},
  };
  std::cout << "capacity: " << customers.capacity() << '\n';
  for (int i = 0; i < numElems; ++i) {
    customers.push_back(Customer(std::string("Cust") + std::to_string(i)));
    std::cout << "capacity: " << customers.capacity() << '\n';
  }
  addTwice(customers, Customer("Bill Smith", Country::usa));
  std::cout << "capacity: " << customers.capacity() << '\n';

  // assign some values to all customers:
  timer.check("start handling values");
  std::srand(static_cast<unsigned int>(time(NULL)));  // initialize random seed
  std::vector<Customer>::iterator pos;
  for (int i = 0; i < 10; ++i) {
    // for (pos = customers.begin(); pos != customers.end(); ++pos) {
    //  int val = std::rand() % 10 + 1; // "random" number between 1 and 10
    //  pos->addValue(val);
    //}
    for (auto& customer : customers) {
      int val = std::rand() % 10 + 1;  // "random" number between 1 and 10
      customer.addValue(val);
    }
  }

  // print the collection:
  timer.check("print customers");
  printColl(customers);

  // sort customers according to a sorting criterion:
  timer.check("sort customers according to sum of values");
  std::sort(customers.begin(), customers.end(),  // range to sort
            lessCustomerValues);                 // sorting criterion

  // print the sorted collection:
  timer.check("print customers sorted according to values");
  printColl(customers);

  // find all customers that have at least one 10 as value and count them:
  // "How much customers have the value 10?"
  for (int val : createCustomer().getValues()) {
    std::cout << "val: " << val << '\n';
  }
  int numCustomersWith10 = 0;
  for (const auto& customer : customers) {
    for (const auto& value : customer.getValues()) {
      if (value == 10) {
        ++numCustomersWith10;
        break;  // break the INNER loop so that we don't count the next 10
      }
    }
  }
  std::cout << "we have " << numCustomersWith10
            << " customers with a value 10\n";

  // create map to find customers by name:
  timer.check("create collection to map names to persons");
  typedef std::unordered_multimap<std::string, Person*> StrPersonMap;
  StrPersonMap people;
  for (pos = customers.begin(); pos != customers.end(); ++pos) {
    people.insert(make_pair(pos->getName(), &(*pos)));
  }

  printHashTableState(people);

  // start loop to find customers by name:
  timer.check("start interactive loop");
  for (;;) {
    // read name:
    std::string name;
    std::cout << "\nName: ";
    std::getline(std::cin, name);
    if (name.empty()) {
      break;  // empty input => BREAK THE LOOP
    }
    // print customers for each found name:
    StrPersonMap::iterator foundPos = people.find(name);
    std::cout << "pair<name,Person*> is at: " << &(*foundPos) << '\n';
    if (foundPos == people.end()) {
      std::cout << "no customer found" << '\n';
    } else {
      // iterate over all found customers with this name:
      for (; foundPos != people.end() && foundPos->first == name; ++foundPos) {
        Customer* cp = dynamic_cast<Customer*>(foundPos->second);
        if (cp) {
          std::cout << "sum: " << cp->getSum() << '\n';
        }
      }
    }
  }

  return 0;
}

}  // namespace customer

// TEST---------------------------------------------------------------------------------------------------------------|
#include "gtest/gtest.h"

namespace {

using namespace customer;

int my_argc;
const char** my_argv;

TEST(Test, Test1) { main(my_argc, my_argv); }

}  // namespace
