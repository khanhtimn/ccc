#include <cstddef>
#include <cstdio>
#include <new>

template <size_t Size>
struct Bucket {
  static const size_t data_size{ Size };
  std::byte data[data_size];
};

struct Heap {
  using SmallBucket = Bucket<4096>;
  using LargeBucket = Bucket<1048576>;

  void* allocate(size_t bytes) {
    if(bytes > LargeBucket::data_size) {
      throw std::bad_alloc();
    } else if(bytes > SmallBucket::data_size) {
      for(size_t i{ }; i < n_large_buckets; i++) {
        if(!large_bucket_used[i]) {
          large_bucket_used[i] = true;
          return large_buckets[i].data;
        }
      }
    } else {
      for(size_t i{ }; i < n_small_buckets; i++) {
        if(!small_bucket_used[i]) {
          small_bucket_used[i] = true;
          return small_buckets[i].data;
        }
      }
    }
    throw std::bad_alloc();
  }

  void free(void* p) {
    for(size_t i{ }; i < n_small_buckets; i++) {
      if(small_buckets[i].data == p) {
        small_bucket_used[i] = false;
        return;
      }
    }
    for(size_t i{ }; i < n_large_buckets; i++) {
      if(large_buckets[i].data == p) {
        large_bucket_used[i] = false;
        return;
      }
    }
  }

  static const size_t n_small_buckets{ 10 };
  SmallBucket small_buckets[n_small_buckets]{ };
  bool small_bucket_used[n_small_buckets]{ };

  static const size_t n_large_buckets{ 10 };
  LargeBucket large_buckets[n_large_buckets]{ };
  bool large_bucket_used[n_large_buckets]{ };
};

Heap heap;

void* operator new(size_t n_bytes) {
  return heap.allocate(n_bytes);
}

void operator delete(void* p) noexcept {
  heap.free(p);
}

int main() {
  printf("Small Buckets: %p\n", heap.small_buckets);
  printf("Large Buckets: %p\n", heap.large_buckets);

  auto breakfast = new unsigned int{ 0xC0FFEE };
  auto dinner = new unsigned int{ 0xDEADBEEF };
  printf("Breakfast: %p 0x%x\n", breakfast, *breakfast);
  printf("Dinner:    %p 0x%x\n", dinner, *dinner);
  delete breakfast;
  delete dinner;

  try {
    printf("Allocating over 4096 bytes...\n");
    auto large_array = new char[5000];
    printf("Large array: %p\n", large_array);
    delete[] large_array;
    printf("Freed large array.\n");
  } catch(const std::bad_alloc&) {
    printf("Failed to allocate 5000 bytes.\n");
  }

  try {
    printf("Allocating over 1MB...\n");
    auto huge = new char[1048577];
    printf("Huge array: %p\n", huge);
    delete[] huge;
  } catch(const std::bad_alloc&) {
    printf("std::bad_alloc caught.\n");
  }

  try {
    while(true) {
      new char;
      printf("Allocated a char.\n");
    }
  } catch(const std::bad_alloc&) {
    printf("std::bad_alloc caught.\n");
  }
}
