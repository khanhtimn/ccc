/*
“3-3. Add an Element* previous to Listing 3-9 to make a doubly linked list.
Add an insert_before method to Element.
Traverse the list from front to back, then from back to front, using two separate for loops.
Print the operating_number inside each loop.”
*/

#include <cstdio>

struct Element {
  Element* next{ };
  Element* previous{ };
  Element(char first, char last, short number)
      : prefix{ first, last }
      , operating_number{ number } { }
  void insert_before(Element* new_element) {
    new_element->next = this;
    new_element->previous = this->previous;
    if(this->previous) {
      this->previous->next = new_element;
    }
    this->previous = new_element;
  }
  void insert_after(Element* new_element) {
    new_element->next = this->next;
    new_element->previous = this;
    if(this->next) {
      this->next->previous = new_element;
    }
    this->next = new_element;
  }
  void traverse() {
    Element* front = this;
    while(front->previous) {
      front = front->previous;
    }

    printf("Front to back:\n");
    for(Element* element = front; element; element = element->next) {
      printf("stormtrooper %c%c-%d\n", element->prefix[0], element->prefix[1], element->operating_number);
    }

    // Find the back
    Element* back = this;
    while(back->next) {
      back = back->next;
    }

    printf("Back to front:\n");
    for(Element* element = back; element; element = element->previous) {
      printf("stormtrooper %c%c-%d\n", element->prefix[0], element->prefix[1], element->operating_number);
    }
  }

  private:
  const char prefix[2];
  const short operating_number;
};

int main() {
  Element trooper1{ 'T', 'K', 69 }, trooper2{ 'Q', 'K', 67 }, trooper3{ 'T', 'P', 3 };
  trooper1.insert_after(&trooper2);
  trooper2.insert_before(&trooper3);
  trooper1.traverse();
  return 0;
}