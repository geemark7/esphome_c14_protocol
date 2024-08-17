#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/select/select.h"
//#include "esphome/components/switch/switch.h"
#include "../c14_protocol/c14_protocol.h"
        
namespace esphome {
namespace c14_nano1 {

static const char *const S_OPERATING_MODE_STR[7] = 
  {
    "Manual 3rd speed",
    "Manual 2nd speed",
    "Manual 1st speed",
    "Manual Stop",
    "Schedule",
    "Holiday",
    "Outdoors"
  };

class C14Nano1: public PollingComponent, public c14_protocol::C14ProtocolDevice {
 public:

  void set_online_status_binary_sensor(binary_sensor::BinarySensor *online_status_binary_sensor) {
    online_status_binary_sensor_ = online_status_binary_sensor;
  }       
  void set_temperature_air_setting_sensor(sensor::Sensor *temperature_air_setting_sensor) {
    temperature_air_setting_sensor_ = temperature_air_setting_sensor;
  }
  void set_temperature_air_exhaust_sensor(sensor::Sensor *temperature_air_exhaust_sensor) {
    temperature_air_exhaust_sensor_ = temperature_air_exhaust_sensor;
  }
  void set_temperature_air_vent_sensor(sensor::Sensor *temperature_air_vent_sensor) {
    temperature_air_vent_sensor_ = temperature_air_vent_sensor;
  }  
  void set_temperature_air_intake_sensor(sensor::Sensor *temperature_air_intake_sensor) {
    temperature_air_intake_sensor_ = temperature_air_intake_sensor;
  }  
  void set_temperature_air_supply_sensor(sensor::Sensor *temperature_air_supply_sensor) {
    temperature_air_supply_sensor_ = temperature_air_supply_sensor;
  }  
  void set_temperature_air_vent_1_sensor(sensor::Sensor *temperature_air_vent_1_sensor) {
    temperature_air_vent_1_sensor_ = temperature_air_vent_1_sensor;
  }  
  void set_temperature_air_intake_1_sensor(sensor::Sensor *temperature_air_intake_1_sensor) {
    temperature_air_intake_1_sensor_ = temperature_air_intake_1_sensor;
  }  
  
  void set_errors_text_sensor(text_sensor::TextSensor *errors_text_sensor) { errors_text_sensor_ = errors_text_sensor; }
  
  void set_operating_mode_select(select::Select *operating_mode_select) {
    operating_mode_select_ = operating_mode_select;
  }
  
   
  void dump_config() override;
  void on_c14_protocol_data(const uint8_t &function, const std::vector<uint8_t> &data) override;
  void update() override;
  void set_operating_mode(uint8_t value);
    
 protected:
  binary_sensor::BinarySensor *online_status_binary_sensor_;
  sensor::Sensor *temperature_air_setting_sensor_;
  sensor::Sensor *temperature_air_exhaust_sensor_;
  sensor::Sensor *temperature_air_vent_sensor_;
  sensor::Sensor *temperature_air_intake_sensor_;
  sensor::Sensor *temperature_air_supply_sensor_;
  sensor::Sensor *temperature_air_vent_1_sensor_;
  sensor::Sensor *temperature_air_intake_1_sensor_;
  text_sensor::TextSensor *errors_text_sensor_;
  select::Select *operating_mode_select_;
  
  uint8_t no_response_count_{0};
  uint8_t update_stage_{0};
  
  void on_data_(const std::vector<uint8_t> &data);
  void on_regs_(const std::vector<uint8_t> &data);
  
  void publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state);
  void publish_state_(sensor::Sensor *sensor, float value);
  //void publish_state_(switch_::Switch *obj, const bool &state);
  void publish_state_(text_sensor::TextSensor *text_sensor, const std::string &state);
  void publish_state_(select::Select *select, const std::string &state);
  void publish_device_unavailable_();
  void reset_online_status_tracker_();
  void track_online_status_();  
  
};

}  // namespace c14_nano1
}  // namespace esphome
  