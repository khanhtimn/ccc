#include <chrono>
#include <cstddef>
#include <iostream>
#include <ostream>

struct TimerClass {
  const char* name;

  TimerClass(const char* name)
      : timestamp{ std::chrono::system_clock::now() }
      , name{ name } {
    std::cout << "Constructing " << name << std::endl;
  }

  // Copy semantics
  TimerClass(const TimerClass& other)
      : timestamp{ other.timestamp }
      , name{ other.name } { }

  TimerClass& operator=(const TimerClass& other) {
    if(this == &other) {
      return *this;
    }

    this->timestamp = other.timestamp;
    this->name = other.name;

    return *this;
  }

  // Move semantics
  TimerClass(TimerClass&& other) noexcept
      : timestamp{ std::move(other.timestamp) }
      , name{ std::move(other.name) } {
    other.timestamp = std::chrono::system_clock::time_point{ };
    other.name = nullptr;
  }

  TimerClass& operator=(TimerClass&& other) noexcept {
    if(this == &other) {
      return *this;
    }
    this->timestamp = other.timestamp;
    this->name = other.name;

    other.timestamp = std::chrono::system_clock::time_point{ };
    other.name = nullptr;

    return *this;
  }

  ~TimerClass() {
    // We reset in move to default constructor of system_clock::time_point
    if(timestamp == std::chrono::system_clock::time_point{ })
      return;

    auto elapsed = std::chrono::system_clock::now() - timestamp;

    std::cout << "Destroying " << name << std::endl;
    std::cout << "[" << name << "] Elapsed: " << elapsed.count() << " ns" << std::endl;
    std::cout << std::endl;
  }

  private:
  std::chrono::system_clock::time_point timestamp;
};

void heavy_compute(TimerClass timer) {
  volatile double result = 0;
  for(long i = 0; i < 200'000'000; ++i) {
    result += std::sqrt(static_cast<double>(i));
  }
  std::cout << "[A heavy computation from " << timer.name << "] result = " << result << std::endl;
}

int main() {
  auto timer1 = TimerClass{ "Timer #1" };
  auto timer2 = TimerClass{ "Timer #2" };
  heavy_compute(std::move(timer1));
  heavy_compute(timer2);

  return 0;
}