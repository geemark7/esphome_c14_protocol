#pragma once

#include "esphome/components/select/select.h"
#include "../c14_nano1.h"


namespace esphome {
namespace c14_nano1 {


class OperatingModeSelect : public select::Select, public Parented<C14Nano1> {
 public:
  OperatingModeSelect() = default;

 protected:
  void control(const std::string &value) override;
};



}  // namespace c14_nano1
}  // namespace esphome