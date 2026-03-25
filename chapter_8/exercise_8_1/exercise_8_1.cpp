#include "fib.hpp"
#include <cstdio>

int main() {
  for(const auto i : Fibonacci::FibonacciRange{ 5000 }) {
    printf("%d ", i);
  }
}
