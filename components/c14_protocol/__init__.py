import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ADDRESS, CONF_ID

DEPENDENCIES = ["uart"]
MULTI_CONF = True

c14_protocol_ns = cg.esphome_ns.namespace("c14_protocol")
C14Protocol = c14_protocol_ns.class_("C14Protocol", cg.Component, uart.UARTDevice)
C14ProtocolDevice = c14_protocol_ns.class_("C14ProtocolDevice")

CONF_C14_PROTOCOL_ID = "c14_protocol_id"
CONF_RX_TIMEOUT = "rx_timeout"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(C14Protocol),
            cv.Optional(
                CONF_RX_TIMEOUT, default="50ms"
            ): cv.positive_time_period_milliseconds,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    cg.add_global(c14_protocol_ns.using)
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    await uart.register_uart_device(var, config)

    cg.add(var.set_rx_timeout(config[CONF_RX_TIMEOUT]))


def c14_protocol_device_schema(default_address):
    schema = {
        cv.GenerateID(CONF_C14_PROTOCOL_ID): cv.use_id(C14Protocol),
    }
    if default_address is None:
        schema[cv.Required(CONF_ADDRESS)] = cv.hex_uint8_t
    else:
        schema[cv.Optional(CONF_ADDRESS, default=default_address)] = cv.hex_uint8_t
    return cv.Schema(schema)


async def register_c14_protocol_device(var, config):
    parent = await cg.get_variable(config[CONF_C14_PROTOCOL_ID])
    cg.add(var.set_parent(parent))
    cg.add(var.set_address(config[CONF_ADDRESS]))
    cg.add(parent.register_device(var))
