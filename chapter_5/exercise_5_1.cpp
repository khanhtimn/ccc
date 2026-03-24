#include <cstdio>
#include <vector>

struct Account {
  Account(const char* name, double amount = 0)
      : name{ name }
      , amount{ amount }
      , id{ next_id++ } { }

  const char* name;
  double amount;
  const long id;

  private:
  inline static long next_id = 0;
};

struct AccountDatabase {
  virtual ~AccountDatabase() = default;
  virtual long retrieve_amount(long id) = 0;
  virtual void set_amount(long id, double amount) = 0;
};

struct InMemoryAccountDatabase : AccountDatabase {
  InMemoryAccountDatabase(std::vector<Account> accounts)
      : accounts{ accounts } { }

  long retrieve_amount(long id) override {
    for(const auto& account : accounts) {
      if(id == account.id) {
        return account.amount;
      }
    }
    return 0;
  }

  void set_amount(long id, double amount) override {
    for(auto& account : accounts) {
      if(id == account.id) {
        account.amount = amount;
      }
    }
  }

  private:
  std::vector<Account> accounts;
};

struct Logger {
  virtual ~Logger() = default;
  virtual void log_transfer(long from, long to, double amount) = 0;
};

struct ConsoleLogger : Logger {
  ConsoleLogger(const char* prefix)
      : prefix{ prefix } { }

  void log_transfer(long from, long to, double amount) override {
    printf("[%s] [cons] %ld -> %ld: %f\n", prefix, from, to, amount);
  }

  private:
  const char* prefix;
};

struct FileLogger : Logger {
  void log_transfer(long from, long to, double amount) override {
    printf("[file] %ld,%ld,%f\n", from, to, amount);
  }
};

struct Bank {
  Bank(AccountDatabase& database)
      : database{ database } { }
  void set_logger(Logger* new_logger) {
    logger = new_logger;
  }
  void make_transfer(long from, long to, double amount) {
    if(logger)
      logger->log_transfer(from, to, amount);
    long balance = database.retrieve_amount(from);
    database.set_amount(from, balance - amount);
    long to_balance = database.retrieve_amount(to);
    database.set_amount(to, to_balance + amount);
  }

  private:
  Logger* logger{ };
  AccountDatabase& database;
};

int main() {
  Account alice{ "Alice", 1000 };
  Account bob{ "Bob", 500 };
  Account carol{ "Carol", 200 };

  InMemoryAccountDatabase db{ { alice, bob, carol } };

  ConsoleLogger console_logger{ "TXN" };
  FileLogger file_logger;

  Bank bank{ db };

  bank.set_logger(&console_logger);
  bank.make_transfer(alice.id, bob.id, 100);

  bank.set_logger(&file_logger);
  bank.make_transfer(bob.id, carol.id, 50);

  bank.set_logger(nullptr);
  bank.make_transfer(carol.id, alice.id, 20);

  printf("Alice: %ld\n", db.retrieve_amount(alice.id));
  printf("Bob:   %ld\n", db.retrieve_amount(bob.id));
  printf("Carol: %ld\n", db.retrieve_amount(carol.id));
}
