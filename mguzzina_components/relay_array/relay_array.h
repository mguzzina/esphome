#pragma once

#include "esphome/core/component.h"
#include "esphome/core/gpio.h"
#include "esphome/components/output/float_output.h"

namespace esphome {
namespace relay_array {

class RelayArrayOutput : public output::FloatOutput, public Component {
 public:
  void set_pins(std::vector<GPIOPin *> pins) { relays_ = pins; }

  void setup() override;
  void dump_config() override;

 protected:
  void write_state(float state) override;

  void activate_relay(size_t active_relays);

  std::vector<GPIOPin *> relays_;
  size_t active_relay_{0};
};

}  // namespace relay_array
}  // namespace esphome
