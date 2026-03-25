#include <vector>

namespace Prime {

struct PrimeIterator {
  PrimeIterator(int start, const std::vector<bool>& sieve)
      : current{ start }
      , sieve{ sieve } {
    while(current < sieve.size() && !sieve[current]) {
      current++;
    }
  }

  bool operator!=(int max) const {
    return current <= max;
  }

  PrimeIterator& operator++() {
    current++; // leave current prime

    while(current < sieve.size()) {
      if(sieve[current]) {
        break; // found next prime?
      }
      current++; // keep walking
    }
    return *this;
  }

  int operator*() const {
    return current;
  }

  private:
  int current;
  const std::vector<bool>& sieve;
};

struct PrimeRange {
  explicit PrimeRange(int max)
      : max{ max }
      , sieve(max + 1, true) {
    if(sieve.size() > 0) {
      sieve[0] = false;
    }
    if(sieve.size() > 1) {
      sieve[1] = false;
    }

    for(int p = 2; p * p <= max; p++) {
      if(sieve[p]) {
        for(int i = p * p; i <= max; i += p) {
          sieve[i] = false;
        }
      }
    }
  }

  PrimeIterator begin() const {
    return PrimeIterator{ 2, sieve };
  }

  int end() const {
    return max;
  }

  private:
  const int max;
  std::vector<bool> sieve;
};

} // namespace Prime