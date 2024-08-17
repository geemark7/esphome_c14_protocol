import esphome.codegen as cg
from esphome.components import select
import esphome.config_validation as cv
from esphome.const import (
    CONF_MODE,
    DEVICE_CLASS_EMPTY,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_NONE,
    ICON_EMPTY,      
    UNIT_EMPTY,
    ENTITY_CATEGORY_CONFIG,
)

from .. import CONF_C14_NANO1_ID, C14_NANO1_COMPONENT_SCHEMA, c14_nano1_ns
              

DEPENDENCIES = ["c14_nano1"]

CODEOWNERS = ["@geemark7"]

  
CONF_OPERATING_MODE = "operating_mode"   # code of register: 108
CONF_OPERATING_MODE_OPTIONS = [
    "Manual 3rd speed",
    "Manual 2nd speed",
    "Manual 1st speed",
    "Manual Stop",
    "Schedule",
    "Holiday",
    "Outdoors"
]
                  


OperatingModeSelect = c14_nano1_ns.class_("OperatingModeSelect", select.Select)


CONFIG_SCHEMA = C14_NANO1_COMPONENT_SCHEMA.extend( 
    {   
        cv.Optional(CONF_OPERATING_MODE): select.select_schema(
            OperatingModeSelect,
            #device_class=DEVICE_CLASS_EMPTY,
            #state_class=STATE_CLASS_NONE,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ), 
    }       
)

                                  
async def to_code(config):                   
    hub = await cg.get_variable(config[CONF_C14_NANO1_ID])
    if conf := config.get(CONF_OPERATING_MODE):
        sel = await select.new_select(conf, options=[CONF_OPERATING_MODE_OPTIONS])
        await cg.register_parented(sel, config[CONF_C14_NANO1_ID])  
        cg.add(hub.set_operating_mode_select(sel))
