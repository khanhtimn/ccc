#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

struct UnsignedBigInteger {
  UnsignedBigInteger()
      : bytes{ } { }
  UnsignedBigInteger(unsigned long long num) {
    do {
      bytes.push_back(static_cast<uint8_t>(num & 0xFF));
      num >>= 8;
    } while(num > 0);
  }

  friend UnsignedBigInteger operator+(const UnsignedBigInteger& lhs, const UnsignedBigInteger& rhs) {
    UnsignedBigInteger result{ };
    size_t max_size = std::max(lhs.bytes.size(), rhs.bytes.size());
    int carry = 0;

    for(size_t i = 0; i < max_size || carry > 0; ++i) {
      int sum = carry;
      if(i < lhs.bytes.size())
        sum += lhs.bytes[i];
      if(i < rhs.bytes.size())
        sum += rhs.bytes[i];

      result.bytes.push_back(static_cast<uint8_t>(sum & 0xFF));
      carry = sum >> 8;
    }
    return result;
  }

  friend UnsignedBigInteger operator-(const UnsignedBigInteger& lhs, const UnsignedBigInteger& rhs) {
    UnsignedBigInteger result{ };
    size_t max_size = std::max(lhs.bytes.size(), rhs.bytes.size());
    int borrow = 0;

    for(size_t i = 0; i < max_size; ++i) {
      int a = (i < lhs.bytes.size()) ? lhs.bytes[i] : 0;
      int b = (i < rhs.bytes.size()) ? rhs.bytes[i] : 0;

      int diff = a - b - borrow;
      if(diff < 0) {
        diff += 256;
        borrow = 1;
      } else {
        borrow = 0;
      }
      result.bytes.push_back(static_cast<uint8_t>(diff));
    }

    if(borrow > 0) {
      throw std::overflow_error("UnsignedBigInteger::operator-: underflow (result is negative)");
    }

    while(result.bytes.size() > 1 && result.bytes.back() == 0) {
      result.bytes.pop_back();
    }
    return result;
  }

  friend UnsignedBigInteger operator*(const UnsignedBigInteger& lhs, const UnsignedBigInteger& rhs) {
    UnsignedBigInteger result{ };
    if(lhs.bytes.empty() || rhs.bytes.empty())
      return result;
    if((lhs.bytes.size() == 1 && lhs.bytes[0] == 0) || (rhs.bytes.size() == 1 && rhs.bytes[0] == 0)) {
      result.bytes.push_back(0);
      return result;
    }

    result.bytes.resize(lhs.bytes.size() + rhs.bytes.size(), 0);

    for(size_t i = 0; i < lhs.bytes.size(); ++i) {
      int carry = 0;
      for(size_t j = 0; j < rhs.bytes.size() || carry > 0; ++j) {
        long long current = result.bytes[i + j];
        current += static_cast<long long>(lhs.bytes[i]) * (j < rhs.bytes.size() ? rhs.bytes[j] : 0) + carry;
        result.bytes[i + j] = static_cast<uint8_t>(current & 0xFF);
        carry = current >> 8;
      }
    }

    while(result.bytes.size() > 1 && result.bytes.back() == 0) {
      result.bytes.pop_back();
    }
    return result;
  }

  friend bool operator<(const UnsignedBigInteger& lhs, const UnsignedBigInteger& rhs) {
    size_t l_size = lhs.bytes.size();
    while(l_size > 1 && lhs.bytes[l_size - 1] == 0)
      --l_size;
    size_t r_size = rhs.bytes.size();
    while(r_size > 1 && rhs.bytes[r_size - 1] == 0)
      --r_size;

    if(l_size != r_size)
      return l_size < r_size;
    for(size_t i = l_size; i > 0; --i) {
      if(lhs.bytes[i - 1] != rhs.bytes[i - 1])
        return lhs.bytes[i - 1] < rhs.bytes[i - 1];
    }
    return false;
  }

  friend bool operator<=(const UnsignedBigInteger& lhs, const UnsignedBigInteger& rhs) {
    return !(rhs < lhs);
  }

  friend bool operator==(const UnsignedBigInteger& lhs, const UnsignedBigInteger& rhs) {
    size_t l_size = lhs.bytes.size();
    while(l_size > 1 && lhs.bytes[l_size - 1] == 0)
      --l_size;
    size_t r_size = rhs.bytes.size();
    while(r_size > 1 && rhs.bytes[r_size - 1] == 0)
      --r_size;

    if(l_size != r_size)
      return false;
    for(size_t i = 0; i < l_size; ++i) {
      if(lhs.bytes[i] != rhs.bytes[i])
        return false;
    }
    return true;
  }

  static void div_mod(const UnsignedBigInteger& dividend,
                      const UnsignedBigInteger& divisor,
                      UnsignedBigInteger& quotient,
                      UnsignedBigInteger& remainder) {
    if(divisor.bytes.empty() || (divisor.bytes.size() == 1 && divisor.bytes[0] == 0)) {
      throw std::invalid_argument("Division by zero");
    }

    if(dividend < divisor) {
      remainder = dividend;
      quotient.bytes.push_back(0);
      return;
    }

    quotient.bytes.resize(dividend.bytes.size(), 0);

    for(size_t i = dividend.bytes.size(); i > 0; --i) {
      for(int bit = 7; bit >= 0; --bit) {
        int r_carry = 0;
        for(size_t k = 0; k < remainder.bytes.size(); ++k) {
          int next_carry = (remainder.bytes[k] & 0x80) ? 1 : 0;
          remainder.bytes[k] = static_cast<uint8_t>((remainder.bytes[k] << 1) | r_carry);
          r_carry = next_carry;
        }
        if(r_carry > 0)
          remainder.bytes.push_back(1);

        if((dividend.bytes[i - 1] >> bit) & 1) {
          if(remainder.bytes.empty())
            remainder.bytes.push_back(1);
          else
            remainder.bytes[0] |= 1;
        }

        while(remainder.bytes.size() > 1 && remainder.bytes.back() == 0)
          remainder.bytes.pop_back();

        if(divisor <= remainder) {
          remainder = remainder - divisor;
          quotient.bytes[i - 1] |= (1 << bit);
        }
      }
    }
    while(quotient.bytes.size() > 1 && quotient.bytes.back() == 0) {
      quotient.bytes.pop_back();
    }
    if(remainder.bytes.empty())
      remainder.bytes.push_back(0);
  }

  friend UnsignedBigInteger operator/(const UnsignedBigInteger& lhs, const UnsignedBigInteger& rhs) {
    UnsignedBigInteger q{ }, r{ };
    div_mod(lhs, rhs, q, r);
    return q;
  }

  friend UnsignedBigInteger operator%(const UnsignedBigInteger& lhs, const UnsignedBigInteger& rhs) {
    UnsignedBigInteger q{ }, r{ };
    div_mod(lhs, rhs, q, r);
    return r;
  }

  explicit operator int() const {
    unsigned long long sum = 0;
    for(size_t i = 0; i < bytes.size(); ++i) {
      if(i >= sizeof(int) && bytes[i] != 0) {
        throw std::overflow_error("UnsignedBigInteger::operator int: narrowing would lose data");
      }
      sum |= (static_cast<unsigned long long>(bytes[i]) << (i * 8));
    }

    if(sum > static_cast<unsigned long long>(std::numeric_limits<int>::max())) {
      throw std::overflow_error("UnsignedBigInteger::operator int: value too large for signed int");
    }
    return static_cast<int>(sum);
  }

  friend std::ostream& operator<<(std::ostream& os, const UnsignedBigInteger& num) {
    if(num.bytes.empty())
      return os << "0";

    std::vector<uint8_t> temp = num.bytes;
    std::string result_str;

    while(!temp.empty() && temp.back() == 0) {
      temp.pop_back();
    }

    if(temp.empty())
      return os << "0";

    while(!temp.empty()) {
      int remainder = 0;
      for(auto it = temp.rbegin(); it != temp.rend(); ++it) {
        int current = remainder * 256 + *it;
        *it = static_cast<uint8_t>(current / 10);
        remainder = current % 10;
      }
      result_str.push_back(static_cast<char>('0' + remainder));

      while(!temp.empty() && temp.back() == 0) {
        temp.pop_back();
      }
    }

    for(auto it = result_str.rbegin(); it != result_str.rend(); ++it) {
      os << *it;
    }

    return os;
  }

  private:
  std::vector<uint8_t> bytes;
};

int main() {
  UnsignedBigInteger a{ 100'000'000'000'000'000 };
  UnsignedBigInteger b{ 200'000'000'000'000'000 };

  std::cout << "a: " << a << std::endl;
  std::cout << "b: " << b << std::endl;

  UnsignedBigInteger c{ a + b };
  std::cout << "a + b = " << c << std::endl;

  UnsignedBigInteger d{ b - a };
  std::cout << "b - a = " << d << std::endl;

  std::cout << "a * 2 = " << a * 2 << std::endl;
  std::cout << "3 * a = " << 3 * a << std::endl;
  std::cout << "b / a = " << b / a << std::endl;
  std::cout << "b % a = " << b % a << std::endl;

  try {
    auto f = static_cast<int>(a);
    std::cout << "f: " << f << std::endl;
  } catch(const std::exception& ex) {
    std::cout << "Caught exception: " << ex.what() << std::endl;
  }

  return 0;
}