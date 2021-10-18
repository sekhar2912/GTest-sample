#include <iostream>
#include "Analyzer.hpp"

using namespace  std;

  int Analyzer::fib(int input)
  {
      if (input <= 1)
          return input;
      return fib(input - 1) + fib(input - 2);
  }