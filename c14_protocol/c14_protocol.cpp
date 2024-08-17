#include "c14_protocol.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace c14_protocol {

static const char *const TAG = "c14_protocol";

static const uint8_t FUNCTION_WRITE_REGISTERS = 'W';
static const uint8_t FUNCTION_READ_REGISTERS = 'R';
static const uint8_t FUNCTION_TEMPERATURES = 'T';



void C14Protocol::loop() {
  const uint32_t now = millis();
  if (now - this->last_c14_protocol_byte_ > this->rx_timeout_) {
    if (this->rx_buffer_.size() > 0) {
      ESP_LOGV(TAG, "Buffer cleared due to timeout: %s",
                format_hex_pretty(&this->rx_buffer_.front(), this->rx_buffer_.size()).c_str());
      this->rx_buffer_.clear();
    }
    this->last_c14_protocol_byte_ = now;
  }

  while (this->available()) {
    uint8_t byte;
    this->read_byte(&byte);
    if (this->parse_c14_protocol_byte_(byte)) {
      this->last_c14_protocol_byte_ = now;
    } else {
      ESP_LOGV(TAG, "Buffer cleared due to reset: %s",
                format_hex_pretty(&this->rx_buffer_.front(), this->rx_buffer_.size()).c_str());
      this->rx_buffer_.clear();
    }
  }
}

uint8_t chksum(const uint8_t data[], const uint16_t len) {
  uint16_t checksum = 0;
  for (uint16_t i = 0; i < len; i++) {
    if (i != 2) checksum = checksum + data[i];
  }
  return checksum & 127;
}

bool C14Protocol::parse_c14_protocol_byte_(uint8_t byte) {
  size_t at = this->rx_buffer_.size();
  this->rx_buffer_.push_back(byte);
  const uint8_t *raw = &this->rx_buffer_[0];

  if (at < 29)
    return true;

  // Byte 0: receipient address
  uint8_t to_address = raw[0] - 128;
  // Byte 1: Response code
  uint8_t function = raw[1];
  // Byte 2: check  sum
  uint8_t remote_crc = raw[2];
  // Byte 3: sender address
  uint8_t from_address = raw[3];
  // Byte 4: Reserved

  // frame length
  if (at == 29) {
    if (raw[29] != '#') {
      ESP_LOGW(TAG, "Incorrect delimiter: 0x%02X", raw[29]);
      return false;
    }
  }

  uint8_t computed_crc = chksum(raw, 30);
  if (computed_crc != remote_crc) {
    ESP_LOGW(TAG, "CRC check failed! 0x%02X != 0x%02X", computed_crc, remote_crc);
    return false;
  }

  ESP_LOGV(TAG, "Received C14Protocol frame from address 0x%02X to address 0x%02X", from_address, to_address);

  if ((to_address != 113))  {
     ESP_LOGV(TAG, "Receiver address should be: 0x%02X", 113);
     return false;
  }

  std::vector<uint8_t> data(this->rx_buffer_.begin() + 5, this->rx_buffer_.begin() + 30 - 1);

  bool found = false;
  for (auto *device : this->devices_) {
    if (device->address_ == from_address) {
      ESP_LOGV(TAG, "Frame accepted. Device address: 0x%02X", from_address);
      device->on_c14_protocol_data(function, data);
      found = true;
    }
  }
  if (!found) {
    ESP_LOGW(TAG, "Got C14Protocol frame from unknown address 0x%02X!", from_address);
  }

  // return false to reset buffer
  return false;
}




void C14Protocol::dump_config() {
  ESP_LOGCONFIG(TAG, "C14Protocol:");
  ESP_LOGCONFIG(TAG, "  RX timeout: %d ms", this->rx_timeout_);
}

float C14Protocol::get_setup_priority() const {
  // After UART bus
  return setup_priority::BUS - 1.0f;
}

void C14Protocol::send(uint8_t address, uint8_t function, const std::vector<uint16_t> &a, const std::vector<uint16_t> &d) {
  uint8_t frame[30];
  frame[0] = address  + 128;   // adress odbiorcy 
  frame[1] = function;  // function
  frame[2] = 0x00;      // checksum
  frame[3] = 0x71;      // from address (personal computer)
  frame[4] = 0x00;      // --  reserved
  frame[5]  = a[0] >> 7;           //  A1 H
  frame[6]  = a[0] & 127;          //  A1 L
  frame[7]  = (d[0]+2000) >> 7;    //  D1 H
  frame[8]  = (d[0]+2000) & 127;   //  D1 L
  frame[9]  = a[1] >> 7;           //  A2 H
  frame[10] = a[1] & 127;          //  A2 L
  frame[11] = (d[1]+2000) >> 7;    //  D2 H
  frame[12] = (d[1]+2000) & 127;   //  D2 L
  frame[13] = a[2] >> 7;           //  A3 H
  frame[14] = a[2] & 127;          //  A3 L
  frame[15] = (d[2]+2000) >> 7;    //  D3 H
  frame[16] = (d[2]+2000) & 127;   //  D3 L
  frame[17] = a[3] >> 7;           //  A4 H
  frame[18] = a[3] & 127;          //  A4 L
  frame[19] = (d[3]+2000) >> 7;    //  D4 H
  frame[20] = (d[3]+2000) & 127;   //  D4 L
  frame[21] = a[4] >> 7;           //  A5 H
  frame[22] = a[4] & 127;          //  A5 L
  frame[23] = (d[4]+2000) >> 7;    //  D5 H
  frame[24] = (d[4]+2000) & 127;   //  D5 L
  frame[25] = a[5] >> 7;           //  A6 H
  frame[26] = a[5] & 127;          //  A6 L
  frame[27] = (d[5]+2000) >> 7;    //  D6 H
  frame[28] = (d[5]+2000) & 127;   //  D6 L
  frame[29] = '#';                 // end sequence
  
  frame[2] = chksum(frame, 30);

  this->write_array(frame, 30);
  this->flush();                                
  
  ESP_LOGV(TAG, "Frame sent to 0x%02X: function %s ", address, function);
}



void C14Protocol::write_registers(uint8_t address, const std::vector<uint16_t> &a, const std::vector<uint16_t> &d) {
  delay(150);  // NOLINT
  this->send(address, FUNCTION_WRITE_REGISTERS, a, d);
}


void C14Protocol::read_temperatures(uint8_t address, const std::vector<uint16_t> &a) {
  delay(150);  // NOLINT   
  std::vector<uint16_t> d = {0,0,0,0,0,0};
  this->send(address, FUNCTION_TEMPERATURES, a, d);
  
}


void C14Protocol::read_registers(uint8_t address, const std::vector<uint16_t> &a) {
  delay(150);  // NOLINT
  std::vector<uint16_t> d = {0,0,0,0,0,0};
  this->send(address, FUNCTION_READ_REGISTERS, a, d);
}

}  // namespace c14_protocol
}  // namespace esphome