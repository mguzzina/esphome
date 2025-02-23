import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import output
from esphome.const import CONF_ID, CONF_PIN

relay_array_ns = cg.esphome_ns.namespace("relay_array")
RelayArrayOutput = relay_array_ns.class_(
    "RelayArrayOutput", output.FloatOutput, cg.Component
)

# ARRAY_SIZE_KEY = "size"
ARRAY_CHANNELS_KEY = "channels"

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend(
    {
        cv.Required(CONF_ID): cv.declare_id(RelayArrayOutput),
        # cv.Required(ARRAY_SIZE_KEY): cv.int_range(1),
        cv.Required(ARRAY_CHANNELS_KEY): cv.ensure_list(
            cv.Schema({cv.Required(CONF_PIN): pins.gpio_output_pin_schema})
        ),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await output.register_output(var, config)
    await cg.register_component(var, config)

    pin_list = []
    for pin_conf in config[ARRAY_CHANNELS_KEY]:
        pin_list.append(await cg.gpio_pin_expression(pin_conf[CONF_PIN]))
    cg.add(var.set_pins(pin_list))
