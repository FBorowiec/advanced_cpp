#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP
#include <cassert>  // for assert()
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

/********************************************
 * Class hierarchy: Person - Customer
 ********************************************/

// helper type for country:
enum class Country { open, germany, usa, uk, other };

// class for Person's having name and country
class Person {
 protected:
  std::string name;
  Country country;

 public:
  Person(const std::string& n, Country c = Country::open)
      : name(n), country(c) {
    assert(!n.empty());
  }

  void setName(const std::string& n) {  // should not be overwritten
    assert(!n.empty());
    name = n;
  }
  std::string getName() const { return name; }
  bool hasCountry() const { return country != Country::open; }

  virtual void toStream(std::ostream& strm) const {
    strm << "[" << name << "]";
  }

  virtual ~Person() {}
  Person(const Person&) = default;
  Person(Person&&) = default;

 protected:
  Person& operator=(const Person&) = default;
  Person& operator=(Person&&) = default;
};

// to be able to: cout << Person(...):
inline std::ostream& operator<<(std::ostream& strm, const Person& p) noexcept {
  p.toStream(strm);
  return strm;
}

// class for Person's being Customer's
class Customer : public Person {
 protected:
  std::vector<int> values;

 public:
  Customer(const std::string& n, Country c = Country::open) : Person(n, c) {}

  void addValue(int val) { values.push_back(val); }
  long getSum() const {
    return std::accumulate(values.begin(), values.end(), 0L);
  }

  const std::vector<int>& getValues() const& { return values; }
  std::vector<int> getValues() && { return std::move(values); }

  virtual void toStream(std::ostream& strm) const {
    strm << "[" << name << " (values:";
    std::vector<int>::const_iterator pos;
    for (pos = values.begin(); pos != values.end(); ++pos) {
      strm << ' ' << *pos;
    }
    strm << ")]";
  }

#ifdef SEE_COPY_AND_MOVE
  // copy constructor and assignment operator:
  Customer(const Customer& c) : Person{c}, values{c.values} {
    std::cout << "COPY " << c << '\n';
  }
  Customer& operator=(const Customer& c) {
    std::cout << "COPYASSIGN " << c << '\n';
    Person::operator=(c);
    values = c.values;
    return *this;
  }
  // move constructor and assignment operator:
  Customer(Customer&& c) noexcept(
      std::is_nothrow_move_constructible<decltype(values)>::value&& noexcept(
          std::cout << c))
      : Person{std::move(c)}, values{std::move(c.values)} {
    std::cout << "MOVE " << *this << '\n';
  }
  Customer& operator=(Customer&& c) {
    std::cout << "MOVEASSIGN " << c << '\n';
    Person::operator=(std::move(c));
    // Person::operator=(static_cast<Customer&&>(c));
    // Person::operator=(static_cast<decltype(c)>(c));
    values = std::move(c.values);
    return *this;
  }
#endif
};

#endif  // CUSTOMER_HPP
