#include "c14_nano1.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace c14_nano1 {

static const char *const TAG = "c14_nano1";

static const uint8_t MAX_NO_RESPONSE_COUNT = 10;

static const uint8_t FUNCTION_WRITE_REGISTERS = 'W';
static const uint8_t FUNCTION_READ_REGISTERS = 'R';
static const uint8_t FUNCTION_TEMPERATURES = 'T';

static const uint8_t FUNCTION_RESP_WRITE_REGISTERS = 'w';
static const uint8_t FUNCTION_RESP_READ_REGISTERS = 'r';
static const uint8_t FUNCTION_RESP_TEMPERATURES = 't';



void C14Nano1::on_c14_protocol_data(const uint8_t &function, const std::vector<uint8_t> &data) {
  
  this->reset_online_status_tracker_();

  ESP_LOGD(TAG, "Received data: %s", format_hex_pretty(data).c_str());
  
  if (function == FUNCTION_RESP_WRITE_REGISTERS) {
    ESP_LOGI(TAG, "Registers 0x%02X updated", data[0]);
    return;
  }

  if (function == FUNCTION_RESP_READ_REGISTERS) {
    this->on_regs_(data);
    return;
  }

  if (function == FUNCTION_RESP_TEMPERATURES) {
    this->on_data_(data);
    return;
  }
 
 
  ESP_LOGW(TAG, "Unhandled response (function %d) received: %s", function, 
           format_hex_pretty(&data.front(), data.size()).c_str());
}



void C14Nano1::on_data_(const std::vector<uint8_t> &data) {

  auto c14_get_a = [&](size_t i) -> uint16_t { return (uint16_t(data[i+0] << 7) | (uint16_t(data[i+1]) & 127)); };
  auto c14_get_d = [&](size_t i) -> uint16_t { return ((uint16_t(data[i+0] << 7) | (uint16_t(data[i+1]) & 127)) - 2000); };

  for (uint8_t i = 0; i < 6; i++) {
    uint16_t a = c14_get_a(i*4);
    uint16_t d = c14_get_d(i*4+2);
    if (a > 0) {
      ESP_LOGD(TAG, "Data from temper. register %d : %d ", a, d);

           if (a == 2)  this->publish_state_(this->temperature_air_setting_sensor_,  d * 0.1f);
      else if (a ==  1) this->publish_state_(this->temperature_air_exhaust_sensor_,  d * 0.1f);
      else if (a == 10) this->publish_state_(this->temperature_air_vent_sensor_,     d * 0.1f);
      else if (a ==  9) this->publish_state_(this->temperature_air_intake_sensor_,   d * 0.1f);
      else if (a == 11) this->publish_state_(this->temperature_air_supply_sensor_,   d * 0.1f);
      else if (a == 13) this->publish_state_(this->temperature_air_vent_1_sensor_,   d * 0.1f);
      else if (a == 14) this->publish_state_(this->temperature_air_intake_1_sensor_, d * 0.1f);
    }
  }
  this->publish_state_(this->errors_text_sensor_, "");
}



void C14Nano1::on_regs_(const std::vector<uint8_t> &data) {

  auto c14_get_a = [&](size_t i) -> uint16_t { return (uint16_t(data[i+0] << 7) | (uint16_t(data[i+1]) & 127)); };
  auto c14_get_d = [&](size_t i) -> uint16_t { return ((uint16_t(data[i+0] << 7) | (uint16_t(data[i+1]) & 127)) - 2000); };

  for (uint8_t i = 0; i < 6; i++) {
    uint16_t a = c14_get_a(i*4);
    uint16_t d = c14_get_d(i*4+2);
    if (a > 0) {
      ESP_LOGD(TAG, "Data from register %d : %d ", a, d);
      if (a == 108) {
        if (this->operating_mode_select_->has_index(d)) {
          this->operating_mode_select_->publish_state(S_OPERATING_MODE_STR[d]);
          //this->publish_state_(this->operating_mode_select_, d);
        }      
      }
    }
  }
  this->publish_state_(this->errors_text_sensor_, "");   
}




void C14Nano1::update() {
  
  
  this->track_online_status_();
  
  ESP_LOGD(TAG, "Updating. Stage %d. Address 0X%02x.", this->update_stage_, this->address_);

  if (this->update_stage_ == 0) {
    std::vector<uint16_t> a = {1,9,11,12,13,14};
    this->read_temperatures( a );
    //this->read_temperatures();
  }        
  else if (this->update_stage_ == 1) {
    std::vector<uint16_t> b = {2,3,4,5,8,10};
    this->read_temperatures( b );
  }
  else if (this->update_stage_ == 2) {
    std::vector<uint16_t> c = {6,7,15,16,17,18};
    this->read_temperatures( c );
  }   
  else if (this->update_stage_ == 3) {
    std::vector<uint16_t> d = {103,104,105,106,107,108};
    this->read_registers( d );
  }/*   
  else if (this->update_stage_ == 4) {
    std::vector<uint16_t> e = {97,98,99,100,101,102};
    this->read_registers( e );
  }   
  else if (this->update_stage_ == 5) {
    std::vector<uint16_t> f = {91,92,93,94,95,96};
    this->read_registers( f );
  } 
  else if (this->update_stage_ == 6) {
    std::vector<uint16_t> g = {109,110,111,112,113,114};
    this->read_registers( g );
  } */
    
  this->update_stage_++;
  if (this->update_stage_ > 3) this->update_stage_ = 0;      
   
}

void C14Nano1::set_operating_mode(uint8_t value) {
  std::vector<uint16_t> a = {108,0,0,0,0,0};
  std::vector<uint16_t> d = {value,0,0,0,0,0};
  this->write_registers( a, d );
}



void C14Nano1::track_online_status_() {
  if (this->no_response_count_ < MAX_NO_RESPONSE_COUNT) {
    this->no_response_count_++;
  }
  if (this->no_response_count_ == MAX_NO_RESPONSE_COUNT) {
    this->publish_device_unavailable_();
    this->no_response_count_++;
  }
}

void C14Nano1::reset_online_status_tracker_() {
  this->no_response_count_ = 0;
  this->publish_state_(this->online_status_binary_sensor_, true);
}

void C14Nano1::publish_device_unavailable_() {
  this->publish_state_(this->online_status_binary_sensor_, false);
  this->publish_state_(this->errors_text_sensor_, "Offline");
   
  this->publish_state_(temperature_air_setting_sensor_    , NAN);
  this->publish_state_(temperature_air_exhaust_sensor_    , NAN);
  this->publish_state_(temperature_air_vent_sensor_       , NAN);
  this->publish_state_(temperature_air_intake_sensor_     , NAN);
  this->publish_state_(temperature_air_supply_sensor_     , NAN);
  this->publish_state_(temperature_air_vent_1_sensor_     , NAN);
  this->publish_state_(temperature_air_intake_1_sensor_   , NAN);
  this->publish_state_(operating_mode_select_             , "");
}

void C14Nano1::publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state) {
  if (binary_sensor == nullptr)
    return;   
  binary_sensor->publish_state(state);
}

void C14Nano1::publish_state_(sensor::Sensor *sensor, float value) {
  if (sensor == nullptr)
    return;
  sensor->publish_state(value);
}
/*
void C14Nano1::publish_state_(switch::Switch *obj, const bool &state) {
  if (obj == nullptr)
    return;     
  obj->publish_state(state);
} */

void C14Nano1::publish_state_(text_sensor::TextSensor *text_sensor, const std::string &state) {
  if (text_sensor == nullptr)
    return;  
  text_sensor->publish_state(state);
}

void C14Nano1::publish_state_(select::Select *select, const std::string &state) {
  if (select == nullptr)
    return;  
  select->publish_state(state);  
}


void C14Nano1::dump_config() {  // NOLINT(google-readability-function-size,readability-function-size)
  ESP_LOGCONFIG(TAG, "C14Nano1:");
  ESP_LOGCONFIG(TAG, "  Address: 0x%02X", this->address_);
  
  LOG_SENSOR("", "  Temperature Air Setting",  this->temperature_air_setting_sensor_);
  LOG_SENSOR("", "  Temperature Air Exhaust",  this->temperature_air_exhaust_sensor_);
  LOG_SENSOR("", "  Temperature Air Vent",     this->temperature_air_vent_sensor_);
  LOG_SENSOR("", "  Temperature Air Intake",   this->temperature_air_intake_sensor_);
  LOG_SENSOR("", "  Temperature Air Supply",   this->temperature_air_supply_sensor_);
  LOG_SENSOR("", "  Temperature Air Vent 1",   this->temperature_air_vent_1_sensor_);
  LOG_SENSOR("", "  Temperature Air Intake 1", this->temperature_air_intake_1_sensor_);

  LOG_SELECT("", "  Operating Mode", this->operating_mode_select_);
}

}  // namespace c14_nano1
}  // namespace esphome