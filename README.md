# ESPHome Word Clock Component

A custom ESPHome component to drive a Word Clock LED matrix. Currently supports German time logic (e.g., "Es ist halb zehn") with regional dialect support (e.g. "viertel vor drei"). The hardware is the [word-clock v1](https://github.com/mplogas/word-clock-v1) I built a few years ago. The LED-converter is re-purposed from its latest iteration: the [word-oclock](https://github.com/mplogas/word-oclock) - a completly rewritten/re-engineered version of the word-clock v1.

## Usage

1. Ensure the `components/wordclock` directory is present in your ESPHome configuration folder.
2. Configure the component in your YAML file:

```yaml
# Import the local component
external_components:
  - source:
      type: local
      path: components

# Configure the Word Clock
wordclock:
  id: word_clock_display
  time_id: home_time           # ID of your time component
  light_id: word_clock_matrix  # ID of your addressable light component
  switch_id: regional_switch   # ID of switch for regional format (e.g. "Dreiviertel")
  led_count: 121               # Total LEDs (e.g. 11x11)
  language: "de"
```

## Configuration Variables

- **time_id**: The ID of the time source (required).
- **light_id**: The ID of the addressable light to control (required).
- **switch_id**: The ID of a switch to toggle regional formatting (optional).
- **led_count**: Total number of LEDs in the strip.
- **language**: Language for time conversion (currently only "de").
