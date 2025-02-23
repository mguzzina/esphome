#include "esphome/core/log.h"
#include "relay_array.h"

namespace esphome {
namespace relay_array {

static const char *TAG = "relay_array.output";

void RelayArrayOutput::setup() {
  for (const auto &relay : this->relays_) {
    relay->setup();
    relay->pin_mode(gpio::Flags::FLAG_OUTPUT);
    relay->digital_write(false);
  }
}

void RelayArrayOutput::dump_config() { ESP_LOGCONFIG(TAG, "Relay array custom float output"); }

void RelayArrayOutput::write_state(float state) {
  ESP_LOGD(TAG, "state %f", state);

  if (state <= 0) {
    // turn off all relays
    activate_relay(0);
    return;
  }

  if (state >= 1) {
    // turn on last relay
    activate_relay(this->relays_.size());
    return;
  }

  // Activate channels k when the output is between [k - 1 / n - 1; k / n - 1)
  auto active_relay = 1 + static_cast<size_t>(state * (this->relays_.size() - 1));
  activate_relay(active_relay);
}

void RelayArrayOutput::activate_relay(size_t relay) {
  ESP_LOGD(TAG, "Active relays %d -> %d", this->active_relay_, relay);
  this->active_relay_ = relay;

  if (relay == 0) {
    for (auto &relay : this->relays_) {
      relay->digital_write(false);
    }
    return;
  } else if (relay >= this->relays_.size()) {
    for (size_t i = 0; i < this->relays_.size(); ++i) {
      this->relays_[i]->digital_write(false);
    }
    this->relays_.back()->digital_write(true);
    return;
  }

  size_t i = 0;
  for (; i < relay - 1; ++i) {
    this->relays_[i]->digital_write(false);
  }

  this->relays_[i]->digital_write(true);
  ++i;

  for (; i < this->relays_.size(); ++i) {
    this->relays_[i]->digital_write(false);
  }
}

}  // namespace relay_array
}  // namespace esphome
