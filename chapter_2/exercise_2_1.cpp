#include <iostream>
#include <ostream>

enum class Operation {
  Add,
  Subtract,
  Multiply,
  Divide,
};

struct Calculator {
  Calculator(Operation op) {
    op_ = op;
  }
  int calculate(int a, int b) {
    switch(op_) {
    case(Operation::Add): {
      return a + b;
    } break;
    case(Operation::Subtract): {
      return a - b;
    } break;
    case(Operation::Multiply): {
      return a * b;
    } break;
    case(Operation::Divide): {
      return a / b;
    } break;
    default: {
      std::cout << "Unsupported operation!" << std::endl;
    }
    }
  }

  private:
  Operation op_;
};

int main() {
  Calculator cal{ Operation::Add };
  std::cout << cal.calculate(34, 35) << std::endl;
}