#ifndef TIMER_HPP
#define TIMER_HPP

#include <iostream>
#include <string>
#include <ctime>     // for time()
#ifdef _MSC_VER
#include <windows.h> // for GetTickCount()
#endif


/********************************************
 * timer to print elapsed time
 ********************************************/

class Timer
{
  private:
    static long progstart;  // start of program
    long last;              // last call of this timer
    std::string name;       // name of this timer

    static long now() {
#ifdef _MSC_VER
      return GetTickCount();  // ticks in milliseconds
#else
      struct timespec ts;
      clock_gettime(CLOCK_REALTIME, &ts);
      return ts.tv_sec * 1000 + (ts.tv_nsec+500) / 1000000;
#endif
    }

  public:
    // start and end timer:
    Timer(const std::string& n)
     : last(now()), name(n) {
        check("START ");
    }
    ~Timer() {
        check("END ");
    }
    // print elapsed time:
    void check(const std::string& msg) {
      long n = now();
      std::cout << "\n********************************\n";
      std::cout << "* elapsed: " << n - progstart << " ms (delta: " << n - last << " ms)\n";
      std::cout << "* " << name << ": " << msg << "\n";
      std::cout << "********************************\n\n";
      last = n;
    }
};

// NOTE: ODR violation if header included in two different CPP files:
long Timer::progstart = Timer::now();

#endif // TIMER_HPP
