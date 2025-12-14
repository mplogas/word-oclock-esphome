#pragma once

#include "esphome/core/component.h"
#include "esphome/components/time/real_time_clock.h"
#include "esphome/components/light/addressable_light.h"
#include "esphome/components/switch/switch.h"
#include "timeconverter.h"
#include <memory>
#include <vector>
#include <utility>

namespace esphome {
namespace wordclock {

// Main word clock component
class WordClockComponent : public Component {
 public:
  WordClockComponent();
  ~WordClockComponent();


  void setup() override;
  void loop() override;
  void dump_config() override;

  // Setters from YAML configuration
  void set_led_count(uint16_t count) { led_count_ = count; }
  void set_time_id(time::RealTimeClock *time_id) { time_id_ = time_id; }
  void set_light_id(light::AddressableLightState *light_state) { light_state_ = light_state; }
  void set_switch_id(switch_::Switch *sw) { sw_ = sw; }
  void set_language(const std::string &lang) { language_ = lang; }

 private:
  void update_display_();
  void clear_leds_();
  void set_led_range_(uint16_t start, uint16_t length);

  uint16_t led_count_{121}; // 11x11 matrix = 121 LEDs
  bool use_regional_format_{false};
  std::string language_{"de"};
  
  uint32_t last_update_{0};
  uint32_t update_interval_{1000};  // Update every 1 second

  time::RealTimeClock *time_id_{nullptr};
  light::AddressableLightState *light_state_{nullptr};
  light::AddressableLight *light_{nullptr};
  switch_::Switch *sw_{nullptr};
  std::unique_ptr<ITimeConverter> converter_;

  bool last_light_on_{false};
};

}  // namespace wordclock
}  // namespace esphome
