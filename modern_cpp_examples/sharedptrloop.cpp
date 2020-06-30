#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace sharedptrloop {

// Small class which will be used by a shared_ptr
class Object {
 public:
  // initialize one counter per thread:
  static std::vector<long long> threadLocalCounter;
  static void initThreadLocalCounter(size_t numThreads) {
    threadLocalCounter.resize(numThreads, 0);
  }

  // increment thread local counter:
  void threadLocalCall(size_t thread = 0) { ++threadLocalCounter[thread]; }
};
std::vector<long long> Object::threadLocalCounter;

// number of iterations and print interval:
const int defaultNumElements = 1000;       // 1000 elements
const int defaultNumIterations = 1000000;  // 1 million times loop over
const int printInterval = 100000;
int numElements = defaultNumElements;
int numIterations = defaultNumIterations;

// vector with numElements elements:
std::vector<std::shared_ptr<Object>> coll;

//------------------------------------------------------------------------------
// loop passing shared pointers by value
//------------------------------------------------------------------------------
void loopPassingSharedPtrByValue(int numThreads, size_t threadNo,
                                 long long& result) {
  Object::threadLocalCounter[threadNo] = 0;
  for (int i = 0; i < numIterations / numThreads; ++i) {
    if (i % printInterval == 0) {
      std::cout << "Thread #" << threadNo << ": iteration " << i
                << " (pass shared_ptr by value)" << std::endl;
    }
    for (auto sp : coll) {
      sp->threadLocalCall(threadNo);
    }
  }
  result = Object::threadLocalCounter[threadNo];
}

//------------------------------------------------------------------------------
// loop passing shared pointers by reference
//------------------------------------------------------------------------------
void loopPassingSharedPtrByRef(int numThreads, size_t threadNo,
                               long long& result) {
  Object::threadLocalCounter[threadNo] = 0;
  for (int i = 0; i < numIterations / numThreads; ++i) {
    if (i % printInterval == 0) {
      std::cout << "Thread #" << threadNo << ": iteration " << i
                << " (pass shared_ptr by reference)" << std::endl;
    }
    for (auto& sp : coll) {
      sp->threadLocalCall(threadNo);
    }
  }
  result = Object::threadLocalCounter[threadNo];
}

//------------------------------------------------------------------------------
// loop passing raw pointers
//------------------------------------------------------------------------------
void loopPassingSharedPtrByRawPtr(int numThreads, size_t threadNo,
                                  long long& result) {
  Object::threadLocalCounter[threadNo] = 0;
  for (int i = 0; i < numIterations / numThreads; ++i) {
    if (i % printInterval == 0) {
      std::cout << "Thread #" << threadNo << ": iteration " << i
                << " (pass shared_ptr by raw pointer)" << std::endl;
    }
    auto end = coll.end();
    for (auto&& pos = coll.begin(); pos != end; ++pos) {
      Object* rp = pos->get();
      rp->threadLocalCall(threadNo);
    }
  }
  result = Object::threadLocalCounter[threadNo];
}

}  // namespace sharedptrloop

// TEST---------------------------------------------------------------------------------------------------------------|
#include "gtest/gtest.h"

namespace {

using namespace sharedptrloop;

//------------------------------------------------------------------------------
// main
//------------------------------------------------------------------------------

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  if (argc < 3 ||
      (argc == 3 && *argv[2] != 'v' && *argv[2] != 'r' && *argv[2] != 'p')) {
    std::cout << "Usage: " << argv[0]
              << " N v|r|p [numIterations] [numElements]" << '\n';
    std::cout << "    N - number of threads" << '\n';
    std::cout << "    v - pass std::shared_ptr by value" << '\n';
    std::cout << "    r - pass std::shared_ptr by reference" << '\n';
    std::cout << "    p - pass std::shared_ptr by raw pointer" << '\n';
    return EXIT_FAILURE;
  }
  // number of threads (read from the command line):
  const int numThreads = atoi(argv[1]);
  const size_t numThreadsU = static_cast<size_t>(numThreads);
  if (argc > 3) {
    numIterations = std::atoi(argv[3]);
  }
  if (argc > 4) {
    numElements = std::atoi(argv[4]);
  }

  char style = argv[2][0];
  std::cout << "***************************************************************"
               "*************\n";
  std::cout << "* Start " << numThreads
            << " thread(s) and pass std::shared_ptr ";
  switch (style) {
    case 'v':
      std::cout << "by value" << '\n';
      break;
    case 'r':
      std::cout << "by reference" << '\n';
      break;
    case 'p':
      std::cout << "by raw pointer" << '\n';
      break;
  }
  std::cout << "* and measure " << numIterations << " iterations over "
            << numElements << " elements\n";
  std::cout << "* hardware_concurrency(): "
            << std::thread::hardware_concurrency() << '\n';
  std::cout << "***************************************************************"
               "*************\n";

  coll.reserve(static_cast<size_t>(numElements));
  for (int i = 0; i < numElements; ++i) {
    coll.push_back(std::make_shared<Object>());
  }

  std::vector<std::thread> threads;
  threads.reserve(numThreadsU);
  std::vector<long long> results;
  results.reserve(numThreadsU);

  Object::initThreadLocalCounter(numThreadsU);

  std::cout << "---- initialized" << std::endl;
  auto t0 = std::chrono::high_resolution_clock::now();

  // start threads
  for (size_t threadIdx = 0; threadIdx < numThreadsU; ++threadIdx) {
    switch (style) {
      case 'v':
        threads.push_back(std::thread(loopPassingSharedPtrByValue, numThreads,
                                      threadIdx, std::ref(results[threadIdx])));
        break;
      case 'r':
        threads.push_back(std::thread(loopPassingSharedPtrByRef, numThreads,
                                      threadIdx, std::ref(results[threadIdx])));
        break;
      case 'p':
        threads.push_back(std::thread(loopPassingSharedPtrByRawPtr, numThreads,
                                      threadIdx, std::ref(results[threadIdx])));
        break;
    }
  }

  // wait until all threads are finished
  for (size_t i = 0; i < numThreadsU; ++i) {
    threads[i].join();
  }

  std::cout << "---- done" << std::endl;
  auto t1 = std::chrono::high_resolution_clock::now();
  auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);

  for (size_t i = 0; i < numThreadsU; ++i) {
    std::cout << "thread " << i << " result: " << results[i] << std::endl;
  }
  std::cout << diff.count() << " ms\n";

  return RUN_ALL_TESTS();
  ;
}

int my_argc;
char** my_argv;

TEST(Test, Test1) { main(my_argc, my_argv); }

}  // namespace
