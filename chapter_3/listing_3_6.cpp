/*
Add a read_from and a write_to function to Listing 3-6.
These functions should read or write to upper or lower as appropriate.
Perform bounds checking to prevent buffer overflows.
*/
#include <cstdio>
#include <stdexcept>

void read_from(char* buffer, size_t size, size_t offset = 0) {
  if(buffer == nullptr || size <= 0 || offset < 0 || offset >= size) {
    throw std::runtime_error{ "Invalid buffer or size or offset" };
  }
  if(offset == 0) {
    for(size_t i = 0; i < size; i++) {
      printf("%c", buffer[i]);
    }
    printf("\n");
  } else {
    printf("%s", buffer + offset);
  }
}

void write_to(char* buffer, size_t size, size_t offset, char value) {
  if(buffer == nullptr || size <= 0 || offset < 0 || offset >= size) {
    throw std::runtime_error{ "Invalid buffer or size or offset" };
  }

  *(buffer + offset) = value;
}

int main() {
  char lower[] = "abc?e";
  char upper[] = "ABC?E";
  char* upper_ptr = upper; // Equivalent: &upper[0]

  read_from(lower, sizeof(lower));
  read_from(upper, sizeof(upper));

  // lower[3] = 'd'; // lower now contains a b c d e \0
  // upper_ptr[3] = 'D'; // upper now contains A B C D E \0
  try {
    write_to(lower, sizeof(lower), 3, 'd');
    write_to(upper, sizeof(upper), 3, 'D');
  } catch(const std::runtime_error e) {
    printf("exception caught with message: %s\n", e.what());
  }

  char letter_d = lower[3]; // letter_d equals 'd'
  char letter_D = upper_ptr[3]; // letter_D equals 'D'

  printf("lower: %s\nupper: %s\n", lower, upper);

  // lower[7] = 'g'; // Super bad. You must never do this.
  try {
    write_to(lower, sizeof(lower), 7, 'g');
  } catch(const std::runtime_error e) {
    printf("exception caught with message: %s\n", e.what());
  }

  read_from(lower, sizeof(lower));
  read_from(upper, sizeof(upper));
}
