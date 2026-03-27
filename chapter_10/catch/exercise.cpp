#include <catch2/catch_all.hpp>
#include <functional>
#include <stdexcept>

using namespace std;

struct SpeedUpdate {
  double velocity_mps;
};

struct CarDetected {
  double distance_m;
  double velocity_mps;
};

struct SpeedLimitDetected {
  double speed_mps;
};

struct BreakCommand {
  double time_to_collision_s;
};

using SpeedUpdateCallback = std::function<void(const SpeedUpdate&)>;
using CarDetectedCallback = std::function<void(const CarDetected&)>;
using SpeedLimitCallback = std::function<void(const SpeedLimitDetected&)>;

struct IServiceBus {
  virtual ~IServiceBus() = default;
  virtual void publish(const BreakCommand&) = 0;
  virtual void subscribe(SpeedUpdateCallback) = 0;
  virtual void subscribe(CarDetectedCallback) = 0;
  virtual void subscribe(SpeedLimitCallback) = 0;
};

struct MockServiceBus : IServiceBus {
  void publish(const BreakCommand& cmd) override {
    commands_published++;
    last_command = cmd;
  };
  void subscribe(SpeedUpdateCallback callback) override {
    speed_update_callback = callback;
  };
  void subscribe(CarDetectedCallback callback) override {
    car_detected_callback = callback;
  };
  void subscribe(SpeedLimitCallback callback) override {
    speed_limit_callback = callback;
  };
  BreakCommand last_command{};
  int commands_published{};
  SpeedUpdateCallback speed_update_callback{};
  CarDetectedCallback car_detected_callback{};
  SpeedLimitCallback speed_limit_callback{};
};

struct AutoBrake {
  AutoBrake(IServiceBus& bus)
      : collision_threshold_s{ 5 }
      , speed_mps{}
      , last_speed_limit{39}
      , speed_limit_detected{false} {
    bus.subscribe([this, &bus](const SpeedUpdate& update) { 
      speed_mps = update.velocity_mps;
      if (speed_limit_detected && speed_mps > last_speed_limit) {
        bus.publish(BreakCommand{ 0 });
      }
    });
    bus.subscribe([this, &bus](const CarDetected& cd) {
      auto relative_velocity_mps = speed_mps - cd.velocity_mps;
      auto time_to_collision_s = cd.distance_m / relative_velocity_mps;
      if(time_to_collision_s > 0 && time_to_collision_s <= collision_threshold_s) {
        bus.publish(BreakCommand{ time_to_collision_s });
      }
    });
    bus.subscribe([this, &bus](const SpeedLimitDetected& limit) {
      last_speed_limit = limit.speed_mps;
      speed_limit_detected = true;
      if (speed_mps > last_speed_limit) {
        bus.publish(BreakCommand{ 0 });
      }
    });
  }
  void set_collision_threshold_s(double x) {
    if(x < 1)
      throw runtime_error{ "Collision less than 1." };
    collision_threshold_s = x;
  }
  double get_collision_threshold_s() const {
    return collision_threshold_s;
  }
  double get_speed_mps() const {
    return speed_mps;
  }
  double get_last_speed_limit() const {
    return last_speed_limit;
  }

  private:
  double collision_threshold_s;
  double speed_mps;
  double last_speed_limit;
  bool speed_limit_detected;
};

TEST_CASE("AutoBrake") {
  MockServiceBus bus{};
  AutoBrake auto_break{ bus };

  SECTION("initializes speed to zero") {
    REQUIRE(auto_break.get_speed_mps() == Catch::Approx(0));
  }

  SECTION("initializes sensitivity to five") {
    REQUIRE(auto_break.get_collision_threshold_s() == Catch::Approx(5));
  }

  SECTION("initializes last speed limit to 39") {
    REQUIRE(auto_break.get_last_speed_limit() == Catch::Approx(39));
  }

  SECTION("throws when sensitivity less than one") {
    REQUIRE_THROWS(auto_break.set_collision_threshold_s(0.5L));
  }

  SECTION("saves speed after update") {
    bus.speed_update_callback(SpeedUpdate{ 100L });
    REQUIRE(100L == auto_break.get_speed_mps());
    bus.speed_update_callback(SpeedUpdate{ 50L });
    REQUIRE(50L == auto_break.get_speed_mps());
    bus.speed_update_callback(SpeedUpdate{ 0L });
    REQUIRE(0L == auto_break.get_speed_mps());
  }

  SECTION("publishes speed limit updates") {
    bus.speed_limit_callback(SpeedLimitDetected{ 50 });
    REQUIRE(auto_break.get_last_speed_limit() == Catch::Approx(50));
    bus.speed_limit_callback(SpeedLimitDetected{ 60 });
    REQUIRE(auto_break.get_last_speed_limit() == Catch::Approx(60));
    bus.speed_limit_callback(SpeedLimitDetected{ 30 });
    REQUIRE(auto_break.get_last_speed_limit() == Catch::Approx(30));
  }

  SECTION("no alert when not imminent") {
    auto_break.set_collision_threshold_s(2L);
    bus.speed_update_callback(SpeedUpdate{ 100L });
    bus.car_detected_callback(CarDetected{ 1000L, 50L });
    REQUIRE(bus.commands_published == 0);
  }

  SECTION("brake command when speed limit decreases below current speed") {
    bus.speed_limit_callback(SpeedLimitDetected{ 35 });
    bus.speed_update_callback(SpeedUpdate{ 30 });
    REQUIRE(bus.commands_published == 0);
    bus.speed_limit_callback(SpeedLimitDetected{ 25 });
    REQUIRE(bus.commands_published == 1);
    REQUIRE(bus.last_command.time_to_collision_s == Catch::Approx(0));
  }
}
