#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

namespace esphome {
namespace c14_protocol {

class C14ProtocolDevice;


class C14Protocol : public uart::UARTDevice, public Component {
 public:
  C14Protocol() = default;

  void loop() override;

  void dump_config() override;

  void register_device(C14ProtocolDevice *device) { this->devices_.push_back(device); }

  float get_setup_priority() const override;
                                                       
  void send(uint8_t address, uint8_t function, const std::vector<uint16_t> &a, const std::vector<uint16_t> &d);
  void write_registers(uint8_t address, const std::vector<uint16_t> &a, const std::vector<uint16_t> &d);
  void read_registers(uint8_t address, const std::vector<uint16_t> &a);
  void read_temperatures(uint8_t address, const std::vector<uint16_t> &a);
  
  void set_rx_timeout(uint16_t rx_timeout) { rx_timeout_ = rx_timeout; }

 protected:
  bool parse_c14_protocol_byte_(uint8_t byte);

  std::vector<uint8_t> rx_buffer_;
  uint16_t rx_timeout_{50};
  uint32_t last_c14_protocol_byte_{0};
  std::vector<C14ProtocolDevice *> devices_;
};


class C14ProtocolDevice {
 public:
  void set_parent(C14Protocol *parent) { parent_ = parent; }
  void set_address(uint8_t address) { address_ = address; }
  virtual void on_c14_protocol_data(const uint8_t &function, const std::vector<uint8_t> &data) = 0;

  void write_registers(const std::vector<uint16_t> &a, const std::vector<uint16_t> &d) { 
    this->parent_->write_registers(this->address_, a, d); 
  }
  void read_registers(const std::vector<uint16_t> &a) {
    this->parent_->read_registers(this->address_, a); 
  }
  void read_temperatures(const std::vector<uint16_t> &a)  { 
    this->parent_->read_temperatures(this->address_, a); 
  }   

 protected:
  friend C14Protocol;

  C14Protocol *parent_;
  uint8_t address_;
};


}  // namespace c14_protocol
}  // namespace esphome