#include <algorithm>
#include <concepts>
#include <cstddef>
#include <unordered_map>

template <std::integral T, size_t Length>
T mode(const T (&values)[Length]) {
  std::unordered_map<T, size_t> runs;

  for(size_t i{ }; i < Length; i++) {
    runs[values[i]]++;
  }

  auto best = std::max_element(runs.begin(), runs.end(), [](auto& a, auto& b) { return a.second < b.second; });

  auto ties = std::count_if(runs.begin(), runs.end(), [&](auto& r) { return r.second == best->second; });

  return ties > 1 ? T{ } : best->first;
}

template <std::integral T, size_t Length>
T mean(const T (&values)[Length]) {
  T result{ };
  for(size_t i{ }; i < Length; i++) {
    result += values[i];
  }
  return result / Length;
}

int main() {
  const size_t nums1[]{ 1, 2, 3, 4, 3 };
  const auto nums1_mean = mean(nums1);
  const auto nums1_mode = mode(nums1);
  printf("size_t mean: %lu\n", nums1_mean);
  printf("size_t mode: %lu\n", nums1_mode);

  const long long nums2[]{ 1, 2, 3, 4, 3, 2, 2, 67, 69 };
  const auto nums2_mean = mean(nums2);
  const auto nums2_mode = mode(nums2);
  printf("long long mean: %lld\n", nums2_mean);
  printf("long long mode: %lld\n", nums2_mode);

  return 0;
}