# ESPHome Word Clock Component

A custom ESPHome component to drive a Word Clock LED matrix. Currently supports German time logic (e.g., "Es ist halb zehn") with regional dialect support (e.g. "viertel vor drei"). The hardware is the [word-clock v1](https://github.com/mplogas/word-clock-v1) I built a few years ago. The LED-converter is re-purposed from its latest iteration: the [word-oclock](https://github.com/mplogas/word-oclock) - a completly rewritten/re-engineered version of the word-clock v1.

## Usage

To use this component in your ESPHome configuration, you need to set up the required dependencies (Time, Light, Switch) and then configure the component itself:

```yaml
# 1. Time Source (Required)
time:
  - platform: homeassistant
    id: home_time
    timezone: "Europe/Berlin"

# 2. Addressable Light (Required)
# Note: restore_mode: RESTORE_DEFAULT_OFF is recommended to prevent flash on boot
light:
  - platform: neopixelbus
    type: GRB
    pin: D4
    num_leds: 121
    name: "Word Clock LEDs"
    id: word_clock_matrix
    variant: WS2812
    restore_mode: RESTORE_DEFAULT_OFF

# 3. Regional Format Switch (Required)
switch:
  - platform: template
    name: "Regional Format"
    id: regional_format_switch
    optimistic: true
    restore_mode: RESTORE_DEFAULT_OFF

# 4. Word Clock Component
external_components:
  - source: github://mplogas/word-oclock-esphome@main
    components: [ wordclock ]

wordclock:
  id: word_clock_display
  time_id: home_time
  light_id: word_clock_matrix
  switch_id: regional_format_switch
  led_count: 121
  language: "de"
```


## Configuration Variables

- **time_id**: The ID of the time source (required).
- **light_id**: The ID of the addressable light to control (required).
- **switch_id**: The ID of a switch to toggle regional formatting (required).
- **led_count**: Total number of LEDs in the strip. (optional, default: 121)
- **language**: Language for time conversion (currently only "de").
