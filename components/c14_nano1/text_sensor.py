import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv
from esphome.const import CONF_ICON, CONF_ID, CONF_PASSWORD, ICON_EMPTY, ICON_TIMELAPSE

from . import CONF_C14_NANO1_ID, C14_NANO1_COMPONENT_SCHEMA

DEPENDENCIES = ["c14_nano1"]

CODEOWNERS = ["@syssi"]


CONF_ERRORS = "errors"

ICON_ERRORS = "mdi:alert-circle-outline"

TEXT_SENSORS = [
    CONF_ERRORS,
]

CONFIG_SCHEMA = C14_NANO1_COMPONENT_SCHEMA.extend(
    {

        cv.Optional(CONF_ERRORS): text_sensor.TEXT_SENSOR_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(text_sensor.TextSensor),
                cv.Optional(CONF_ICON, default=ICON_ERRORS): cv.icon,
            }
        ),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_C14_NANO1_ID])
    for key in TEXT_SENSORS:
        if key in config:
            conf = config[key]
            sens = cg.new_Pvariable(conf[CONF_ID])
            await text_sensor.register_text_sensor(sens, conf)
            cg.add(getattr(hub, f"set_{key}_text_sensor")(sens))