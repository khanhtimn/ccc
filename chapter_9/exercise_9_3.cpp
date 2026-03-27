#include <cstddef>
#include <cstdio>
#include <limits>

template <typename Fn, typename In, typename Out>
constexpr Out fold(Fn function, In* input, size_t length, Out initial) {
  Out result{ initial };
  for(size_t i{ 0 }; i < length; i++) {
    result = function(result, input[i]);
  }
  return result;
}

int main() {
  int data[]{ 100, 200, 300, 400, 500 };
  const size_t data_len = 5;
  auto sum = fold([](auto x, auto y) { return x + y; }, data, data_len, 0);
  auto min = fold([](auto x, auto y) { return x <= y ? x : y; }, data, data_len, std::numeric_limits<int>::max());
  auto max = fold([](auto x, auto y) { return x >= y ? x : y; }, data, data_len, std::numeric_limits<int>::min());
  auto greater_than_200 = fold([](auto acc, auto x) { return x > 200 ? ++acc : acc; }, data, data_len, 0);
  printf("Sum: %d\n", sum);
  printf("Min: %d\n", min);
  printf("Max: %d\n", max);
  printf("Greater than 200: %d\n", greater_than_200);
}