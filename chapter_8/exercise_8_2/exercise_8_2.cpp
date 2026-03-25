#include "prime.hpp"
#include <cstdio>

int main() {
  for(const auto i : Prime::PrimeRange{ 4900 }) {
    printf("%d ", i);
  }
}
