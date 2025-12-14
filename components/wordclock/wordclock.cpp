#include "wordclock.h"
#include "esphome/core/log.h"

namespace esphome
{
    namespace wordclock
    {

        static const char *const TAG = "wordclock";

        // ============================================================================
        // WordClockComponent Implementation
        // ============================================================================

        WordClockComponent::WordClockComponent() {}

        WordClockComponent::~WordClockComponent() {}

        void WordClockComponent::setup()
        {
            ESP_LOGI(TAG, "Setting up Word Clock component");

            if (language_ == "de")
            {
                converter_ = std::make_unique<TimeConverterDE>();
                ESP_LOGD(TAG, "Using German time converter");
            }
            else
            {
                ESP_LOGE(TAG, "Language '%s' not supported, using German", language_.c_str());
                converter_ = std::make_unique<TimeConverterDE>();
            }

            if (!time_id_ || !sw_ || !light_state_)
            {
                if(!time_id_) ESP_LOGE(TAG, "No time source configured!");
                if(!sw_) ESP_LOGE(TAG, "No switch configured!");
                if(!light_state_) ESP_LOGE(TAG, "No light output configured!");
                return;
            }
            else
            {
                ESP_LOGD(TAG, "Configuring required components.");
                auto *output = light_state_->get_output();
                light_ = output ? static_cast<light::AddressableLight *>(output) : nullptr;
                if (!light_)
                {
                    ESP_LOGE(TAG, "Configured light is not addressable!");
                }
            }
        }

        void WordClockComponent::loop()
        {
            uint32_t now = millis();
            if (now - last_update_ < update_interval_)
            {
                return;
            }
            last_update_ = now;

            update_display_();
        }

        void WordClockComponent::dump_config()
        {
            ESP_LOGCONFIG(TAG, "Word Clock Configuration:");
            ESP_LOGCONFIG(TAG, "  LED Count: %d", led_count_);
            ESP_LOGCONFIG(TAG, "  Language: %s", language_.c_str());
        }

        void WordClockComponent::update_display_()
        {
            if (!time_id_ || !light_ || !converter_ || !light_state_ || !sw_)
            {
                ESP_LOGE(TAG, "WordClockComponent not properly configured, cannot update display.");
                return;
            }

            bool light_on = light_state_->remote_values.is_on();

            // If we just transitioned ON -> OFF, clear the strip once
            if (!light_on && last_light_on_)
            {
                ESP_LOGD(TAG, "Light turned OFF, clearing LEDs");
                clear_leds_();
                light_->set_effect_active(false);

                light_->schedule_show();
                last_light_on_ = light_on;
                return;
            } else if (light_on && !last_light_on_)
            {
                ESP_LOGD(TAG, "Light turned ON");
            }

            // Only update if the light is ON in Home Assistant
            if (!light_on)
            {
                ESP_LOGD(TAG, "Light is OFF, skipping update");
                return;
            }

            auto now = time_id_->now();
            if (!now.is_valid())
            {
                ESP_LOGW(TAG, "Current time is not valid, skipping update");
                return;
            }

            last_light_on_ = light_on;

            // Clear all LEDs first
            clear_leds_();

            use_regional_format_ = sw_->state;

            auto led_positions = converter_->convertTime(now.hour, now.minute, use_regional_format_);
            for (const auto &pos : led_positions)
            {
                set_led_range_(pos.first, pos.second);
            }

            // Mark effect as active so light component knows we're controlling it
            if (!light_->is_effect_active())
            {
                light_->set_effect_active(true);
            }
            light_->schedule_show();
        }

    void WordClockComponent::set_led_range_(uint16_t start, uint16_t length)
    {
      if (!light_ || !light_state_)
      {
        return;
      }

      int32_t light_size = light_->size();
      if (light_size <= 0 || length == 0)
      {
        return;
      }

      Color c = color_from_light_color_values(light_state_->remote_values);
      ESP_LOGD(TAG, "Setting LED range %d to %d with color %d,%d,%d", start, length, c.red, c.green, c.blue);

      for (uint16_t i = 0; i < length; i++)
      {
        uint32_t led_idx = static_cast<uint32_t>(start) + i;
        if (led_idx >= led_count_ || led_idx >= static_cast<uint32_t>(light_size))
        {
          continue;
        }
        light_->get(static_cast<int32_t>(led_idx)) = c;
      }
    }

    void WordClockComponent::clear_leds_()
    {
      if (!light_)
      {
        return;
      }

      int32_t light_size = light_->size();
      if (light_size <= 0)
      {
        return;
      }

      uint32_t limit = led_count_;
      if (static_cast<uint32_t>(light_size) < limit)
      {
        limit = static_cast<uint32_t>(light_size);
      }

      for (uint32_t i = 0; i < limit; i++)
      {
        light_->get(static_cast<int32_t>(i)) = Color(0, 0, 0);
      }
    }

    }
} // namespace esphome wordclock