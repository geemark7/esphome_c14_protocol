import esphome.codegen as cg
from esphome.components import sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_POWER,
    DEVICE_CLASS_EMPTY,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_TOTAL_INCREASING,
    ICON_EMPTY,
    UNIT_CELSIUS,
    UNIT_EMPTY,
    UNIT_PERCENT,
    UNIT_WATT,
)

from . import CONF_C14_NANO1_ID, C14_NANO1_COMPONENT_SCHEMA


DEPENDENCIES = ["c14_nano1"]

CODEOWNERS = ["@syssi"]



CONF_WORK_MODE                = "work_mode"                    # code 4

CONF_TEMPERATURE_AIR_SETTING  = "temperature_air_setting"      # code 2
CONF_TEMPERATURE_AIR_EXHAUST  = "temperature_air_exhaust"      # code 1
CONF_TEMPERATURE_AIR_VENT     = "temperature_air_vent"         # code 10 (13)
CONF_TEMPERATURE_AIR_INTAKE   = "temperature_air_intake"       # code 9 (14)
CONF_TEMPERATURE_AIR_SUPPLY   = "temperature_air_supply"       # code 11
CONF_TEMPERATURE_AIR_VENT_1   = "temperature_air_vent_1"       # code 13
CONF_TEMPERATURE_AIR_INTAKE_1 = "temperature_air_intake_1"     # code 14

CONF_TEMPERATURE_12           = "temperature_12"               # code 12
CONF_PARAMETER_3              = "parameter_3"                  # code 3
CONF_PARAMETER_5              = "parameter_5"                  # code 5
CONF_PARAMETER_6              = "parameter_6"                  # code 6
CONF_PARAMETER_7              = "parameter_7"                  # code 7
CONF_PARAMETER_8              = "parameter_8"                  # code 8

     
SENSORS = [
  CONF_WORK_MODE,                
  CONF_TEMPERATURE_AIR_SETTING,  
  CONF_TEMPERATURE_AIR_EXHAUST,
  CONF_TEMPERATURE_AIR_VENT,     
  CONF_TEMPERATURE_AIR_INTAKE,   
  CONF_TEMPERATURE_AIR_SUPPLY,  
  CONF_TEMPERATURE_AIR_VENT_1,   
  CONF_TEMPERATURE_AIR_INTAKE_1, 
                                
  CONF_TEMPERATURE_12,           
  CONF_PARAMETER_3,              
  CONF_PARAMETER_5,              
  CONF_PARAMETER_6,              
  CONF_PARAMETER_7,              
  CONF_PARAMETER_8,                
]  
   
   
CONFIG_SCHEMA = C14_NANO1_COMPONENT_SCHEMA.extend( 
    {
        cv.Optional(CONF_WORK_MODE): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),    

        cv.Optional(CONF_TEMPERATURE_AIR_SETTING): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_EMPTY,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_TEMPERATURE_AIR_EXHAUST): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_EMPTY,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),    
        cv.Optional(CONF_TEMPERATURE_AIR_VENT): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_EMPTY,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),    
        cv.Optional(CONF_TEMPERATURE_AIR_INTAKE): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_EMPTY,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),    
        cv.Optional(CONF_TEMPERATURE_AIR_SUPPLY): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_EMPTY,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),    
        cv.Optional(CONF_TEMPERATURE_AIR_VENT_1): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_EMPTY,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),    
        cv.Optional(CONF_TEMPERATURE_AIR_INTAKE_1): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_EMPTY,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),    
        
        cv.Optional(CONF_TEMPERATURE_12): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            icon=ICON_EMPTY,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),                
        cv.Optional(CONF_PARAMETER_3): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),    
        cv.Optional(CONF_PARAMETER_5): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),    
        cv.Optional(CONF_PARAMETER_6): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),    
        cv.Optional(CONF_PARAMETER_7): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),    
        cv.Optional(CONF_PARAMETER_8): sensor.sensor_schema(
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_EMPTY,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_EMPTY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),    
    }       
)



async def to_code(config):                   
    hub = await cg.get_variable(config[CONF_C14_NANO1_ID])
    for key in SENSORS:
        if key in config:
            conf = config[key]
            sens = await sensor.new_sensor(conf)
            cg.add(getattr(hub, f"set_{key}_sensor")(sens))