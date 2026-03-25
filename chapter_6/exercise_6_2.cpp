#include <concepts>
#include <cstdio>

struct Account {
  virtual ~Account() = default;
  virtual const char* get_name() const = 0;
  virtual double get_amount() = 0;
  virtual void set_amount(double amount) = 0;
};

struct CheckingAccount : Account {
  CheckingAccount(const char* name, double amount)
      : name{ name }
      , amount{ amount } { }

  const char* get_name() const override {
    return name;
  }

  double get_amount() override {
    return amount;
  }

  void set_amount(double amount) override {
    if(amount < this->amount) {
      this->amount = amount - 1.0;
      printf("\n%s's checking was charged a $1.00 check fee! New Balance: $%.2f\n", name, this->amount);
    } else {
      this->amount = amount;
    }
  }

  private:
  const char* name;
  double amount;
};

struct SavingAccount : Account {
  SavingAccount(const char* name, double amount, double interest_rate = 0.05)
      : name{ name }
      , amount{ amount }
      , interest_rate{ interest_rate } { }

  const char* get_name() const override {
    return name;
  }

  double get_amount() override {
    return amount;
  }

  void set_amount(double amount) override {
    if(amount > this->amount) {
      this->amount = amount + (amount * interest_rate);
      printf("\n%s's savings accumulated interest! New Balance: $%.2f\n", name, this->amount);
    } else {
      this->amount = amount;
    }
  }

  private:
  const char* name;
  double amount;
  double interest_rate;
};

template <typename T>
concept AccountType = std::derived_from<T, Account>;

struct Logger {
  virtual ~Logger() = default;
  virtual void log_transfer(const char* from, const char* to, double amount) = 0;
};

struct ConsoleLogger : Logger {
  ConsoleLogger(const char* prefix)
      : prefix{ prefix } { }

  void log_transfer(const char* from, const char* to, double amount) override {
    printf("[%s] [cons] %s -> %s: $%.2f\n", prefix, from, to, amount);
  }

  private:
  const char* prefix;
};

struct FileLogger : Logger {
  void log_transfer(const char* from, const char* to, double amount) override {
    printf("[file] %s,%s,%f\n", from, to, amount);
  }
};

template <AccountType T>
struct Bank {
  void set_logger(Logger* new_logger) {
    logger = new_logger;
  }

  void make_transfer(T& from, T& to, double amount) {
    if(&from != &to && amount > 0) {
      if(from.get_amount() >= amount) {
        if(logger) {
          logger->log_transfer(from.get_name(), to.get_name(), amount);
        }
        from.set_amount(from.get_amount() - amount);
        to.set_amount(to.get_amount() + amount);
      }
    }
  }

  private:
  Logger* logger{ };
};

int main() {
  CheckingAccount tracy{ "Tracy", 800 };
  SavingAccount kane{ "Kane", 200, 0.05 };

  printf("Initial Balances:\n");
  printf("Tracy Checking: $%.2f\n", tracy.get_amount());
  printf("Kane Savings: $%.2f\n", kane.get_amount());

  ConsoleLogger console_logger{ "TXN" };

  Bank<Account> bank;
  bank.set_logger(&console_logger);

  printf("\n--- Making Transfer ---\n");
  bank.make_transfer(tracy, kane, 150);

  printf("\nFinal Result:\n");
  printf("Tracy Checking: $%.2f\n", tracy.get_amount());
  printf("Kane Savings: $%.2f\n", kane.get_amount());

  return 0;
}