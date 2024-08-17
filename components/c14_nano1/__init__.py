import esphome.codegen as cg
from esphome.components import c14_protocol
import esphome.config_validation as cv
from esphome.const import CONF_ID

AUTO_LOAD = ["c14_protocol", "binary_sensor", "sensor", "select", "text_sensor"]
CODEOWNERS = ["@syssi","@geemark7"]
MULTI_CONF = True

CONF_C14_NANO1_ID = "c14_nano1_id"

c14_nano1_ns = cg.esphome_ns.namespace("c14_nano1")
C14Nano1 = c14_nano1_ns.class_("C14Nano1", cg.PollingComponent, c14_protocol.C14ProtocolDevice)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(C14Nano1),
        }
    )
    .extend(cv.polling_component_schema("5s"))
    .extend(c14_protocol.c14_protocol_device_schema(0x29))
)

C14_NANO1_COMPONENT_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_C14_NANO1_ID): cv.use_id(C14Nano1),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await c14_protocol.register_c14_protocol_device(var, config)