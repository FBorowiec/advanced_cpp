#include <chrono>
#include <exception>
#include <future>
#include <iostream>
#include <random>

namespace specexe {

using namespace std;

int quickComputation();  // process result "quick and dirty"

int accurateComputation(int,    // maximum number of iterations in the loop
                        char);  // character to print

int bestResultInTime(int num,  // max num of iterations in accurateComputation()
                     char c)   // character to print in accurateComputation()
{
  // define maximum time slot to return result:
  auto tp = std::chrono::steady_clock::now() + std::chrono::seconds(15);

  // start both a quick and an accurate computation:
  std::cout << "start both accurate and quick computation" << std::endl;
  auto f = std::async(std::launch::async, accurateComputation, num, c);
  int guess = quickComputation();
  std::cout << "\nquick computation done" << std::endl;

  // give accurate computation the rest of the time slot
  // and return the best computation result we have:
  auto s = f.wait_until(tp);
  if (s == std::future_status::ready) {
    std::cout << "\naccurate computation done" << std::endl;
    return f.get();
  } else {
    std::cout << "\naccurate computation not finished in time" << std::endl;
    return guess;
  }
}

int quickComputation() {
  // random-number generator (use c as seed to get different sequences)
  std::default_random_engine dre(44);
  std::uniform_int_distribution<int> id(10, 1000);

  // loop to print character after a random period of time
  for (int i = 0; i < 7; ++i) {
    this_thread::sleep_for(chrono::milliseconds(id(dre)));
    cout.put('q').flush();
  }

  return 50000;
}

int accurateComputation(int num, char c) {
  // loop to print passed character after each sedond
  // - loop at most num time or abortion is signaled
  for (int i = 0; i < num; ++i) {
    this_thread::sleep_for(chrono::seconds(1));
    cout.put(c).flush();
  }

  return 49768;
}

int main(int argc, const char** argv) {
  // command line argumment defines amount of loops/seconds
  // in accurate computation
  int num = argc > 1 ? atoi(argv[1]) : 30;
  try {
    // start a first speculative execution
    std::cout << "\nstart bestResultInTime(" << num << ",'a')" << std::endl;
    int v = bestResultInTime(num, 'a');
    std::cout << "\nbestResultInTime() done => " << v << std::endl;

    // start a second speculative execution
    // std::cout << "\nstart bestResultInTime(" << num << ",'b')" << std::endl;
    // v = bestResultInTime(num,'b');
    // std::cout << "\nbestResultInTim() done => " << v << std::endl;
  } catch (const exception& e) {
    cerr << "EXCEPTION: " << e.what() << endl;
  } catch (...) {
    cerr << "EXCEPTION " << endl;
  }

  return 0;
}

}  // namespace specexe

// TEST---------------------------------------------------------------------------------------------------------------|
#include "gtest/gtest.h"

namespace {

using namespace specexe;

TEST(SpecexeTest, FiveLoopsTest) {
  const char* num_of_loops = "5";
  const char* my_argv[] = {"main", num_of_loops, NULL};

  int my_argc = std::size(my_argv) - 1;
  main(my_argc, my_argv);
}

}  // namespace
