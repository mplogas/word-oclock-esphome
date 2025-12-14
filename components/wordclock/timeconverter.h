#pragma once

#include <vector>
#include <utility>
#include <cstdint>

namespace esphome {
namespace wordclock {

// Time converter interface
class ITimeConverter {
 public:
  virtual ~ITimeConverter() = default;
  virtual std::vector<std::pair<uint16_t, uint16_t>> convertTime(
      const uint8_t hours, const uint8_t minutes, 
      bool is_regional_format) = 0;
};

// German time converter
class TimeConverterDE : public ITimeConverter {
 public:
  TimeConverterDE();
  ~TimeConverterDE();

  std::vector<std::pair<uint16_t, uint16_t>> convertTime(
      const uint8_t hours, const uint8_t minutes, 
      bool is_regional_format) override;

 private:
  // LED positions: {start_index, length}
  static constexpr std::pair<uint16_t, uint16_t> LEDS_ERROR = {0, 1};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_ES = {110, 2};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_IST = {113, 3};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_UHR = {11, 3};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_VOR = {79, 3};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_NACH = {82, 4};

  // Minutes
  static constexpr std::pair<uint16_t, uint16_t> LEDS_FUENF = {117, 4};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_ZEHN = {106, 4};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_VIERTEL = {92, 7};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_DREIVIERTEL = {88, 11};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_ZWANZIG = {99, 7};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_HALB = {66, 4};

  // Hours
  static constexpr std::pair<uint16_t, uint16_t> LEDS_H_EIN = {61, 3};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_H_EINS = {60, 4};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_H_ZWEI = {62, 4};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_H_DREI = {45, 4};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_H_VIER = {33, 4};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_H_FUENF = {51, 4};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_H_SECHS = {16, 5};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_H_SIEBEN = {55, 6};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_H_ACHT = {23, 4};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_H_NEUN = {37, 4};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_H_ZEHN = {27, 4};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_H_ELF = {41, 3};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_H_ZWOELF = {71, 5};

  // Status LEDs
  static constexpr std::pair<uint16_t, uint16_t> LEDS_OPTION_1 = {2, 1};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_OPTION_2 = {4, 1};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_OPTION_3 = {6, 1};
  static constexpr std::pair<uint16_t, uint16_t> LEDS_OPTION_4 = {8, 1};

  std::pair<uint16_t, uint16_t> get_hour_leds_(uint8_t hour);
  std::pair<uint16_t, uint16_t> get_minute_leds_(uint8_t minutes, bool is_regional_format = false);
  std::pair<uint16_t, uint16_t> get_status_leds_(uint8_t status);
};

}  // namespace wordclock
}  // namespace esphome
