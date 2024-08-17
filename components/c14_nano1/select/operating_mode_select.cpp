#include "operating_mode_select.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace c14_nano1 {

static const char *const TAG = "c14_nano1.select";

void OperatingModeSelect::control(const std::string &value) {
  this->publish_state(value);
  auto index = this->index_of(value);
  if (index.has_value()) {
    this->parent_->set_operating_mode(index.value());
  }
}

}  // namespace c14_nano1
}  // namespace esphome