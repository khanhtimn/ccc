#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <cerrno>
#include <cstdio>
#include <memory>

using FileGuard = std::shared_ptr<FILE>;

struct Hal {
  Hal(FileGuard file)
      : file{ file } { }

  ~Hal() noexcept {
    fprintf(file.get(), "Stop, Dave.\n");
  }
  void write_status() {
    fprintf(file.get(), "I'm completely operational.\n");
  };

  void say_hello() {
    fprintf(file.get(), "HELLO DAVE\n");
  }

  bool operator==(const char* str) const {
    rewind(file.get());
    size_t len = strlen(str);
    char buf[4096];
    size_t total = 0;

    while(true) {
      size_t n = fread(buf, 1, sizeof(buf), file.get());
      if(n == 0)
        break;

      if(total + n > len)
        return false; // file is longer
      if(memcmp(buf, str + total, n) != 0)
        return false;
      total += n;
    }

    return total == len; // file is shorter if false
  }

  bool operator!=(const char* str) const {
    return !(*this == str);
  }

  private:
  FileGuard file;
};

TEST_CASE("Hal test") {
  auto file = fopen("HAL9000", "w+");
  REQUIRE(file);
  FileGuard file_guard{ file, fclose };

  auto hal1 = Hal{ file_guard };

  hal1.say_hello();
  REQUIRE(hal1 == "HELLO DAVE\n");

  {
    auto hal2 = Hal{ file_guard };
    hal2.say_hello();
    hal1.write_status();

    REQUIRE(hal1 == "HELLO DAVE\n"
                    "HELLO DAVE\n"
                    "I'm completely operational.\n");
  }

  REQUIRE(hal1 == "HELLO DAVE\n"
                  "HELLO DAVE\n"
                  "I'm completely operational.\n"
                  "Stop, Dave.\n");
}