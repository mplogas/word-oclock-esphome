import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import time, light, switch
from esphome.const import CONF_ID, CONF_TIME_ID, CONF_LIGHT_ID

CODEOWNERS = ["@mplogas"]
DEPENDENCIES = ["time", "light", "switch"]

DOMAIN = "wordclock"
ns = cg.esphome_ns.namespace("wordclock")

WordClockComponent = ns.class_("WordClockComponent", cg.Component)

CONF_LED_COUNT = "led_count"
CONF_LANGUAGE = "language"
CONF_SWITCH_ID = "switch_id"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(WordClockComponent),
    cv.GenerateID(CONF_TIME_ID): cv.use_id(time.RealTimeClock),
    cv.GenerateID(CONF_LIGHT_ID): cv.use_id(light.AddressableLightState),
    cv.GenerateID(CONF_SWITCH_ID): cv.use_id(switch.Switch),
    cv.Optional(CONF_LED_COUNT, default=121): cv.int_range(min=1, max=300),
    cv.Optional(CONF_LANGUAGE, default="de"): cv.one_of("de", lower=True),
}).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    time_id = await cg.get_variable(config[CONF_TIME_ID])
    cg.add(var.set_time_id(time_id))

    light_id = await cg.get_variable(config[CONF_LIGHT_ID])
    cg.add(var.set_light_id(light_id))

    switch_id = await cg.get_variable(config[CONF_SWITCH_ID])
    cg.add(var.set_switch_id(switch_id))

    cg.add(var.set_led_count(config[CONF_LED_COUNT]))
    cg.add(var.set_language(config[CONF_LANGUAGE]))
