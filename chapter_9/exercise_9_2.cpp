#include <cstddef>
#include <cstdio>
#include <unordered_map>

struct LengthHistogram {
  void ingest(const char* x);
  void print() const;

  private:
  std::unordered_map<size_t, size_t> freq{ };
};

void LengthHistogram::ingest(const char* x) {
  size_t index{ };
  while(const auto c = x[index]) {
    index++;
  }
  freq[index]++;
}

void LengthHistogram::print() const {
  for(auto& [len, count] : freq) {
    printf("%zu: ", len);
    for(size_t i{ }; i < count; i++)
      printf("*");
    printf("\n");
  }
}

int main(int argc, char** argv) {
  LengthHistogram hist;
  for(size_t i{ 1 }; i < argc; i++) {
    hist.ingest(argv[i]);
  }
  hist.print();
}