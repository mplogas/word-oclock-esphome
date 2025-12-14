#include "timeconverter.h"

namespace esphome
{
    namespace wordclock
    {

        TimeConverterDE::TimeConverterDE() {}
        TimeConverterDE::~TimeConverterDE() {}

        std::vector<std::pair<uint16_t, uint16_t>> TimeConverterDE::convertTime(const uint8_t hours, const uint8_t minutes, bool is_regional_format)
        {

            std::vector<std::pair<uint16_t, uint16_t>> result;

            uint8_t normalizedHours = (hours % 12 == 0) ? 12 : (hours % 12);
            uint8_t roundedMinutes = (minutes / 5) * 5;
            uint8_t displayHour = (minutes >= 25) ? normalizedHours + 1 : normalizedHours;
            if (displayHour == 13)
            {
                displayHour = 1;
            }

            // Add "es ist"
            result.push_back(LEDS_ES);  // "es"
            result.push_back(LEDS_IST); // "ist"

            // Determine the appropriate phrase
            switch (roundedMinutes)
            {
            case 0:
                // "es ist [hour] uhr"
                if (displayHour == 1)
                {
                    result.push_back(LEDS_H_EIN); // "ein"
                }
                else
                {
                    result.push_back(get_hour_leds_(displayHour)); // Hour word
                }
                result.push_back(LEDS_UHR); // "uhr"
                break;

            case 15:
                result.push_back(LEDS_VIERTEL); // "viertel"
                if (is_regional_format)
                {
                    uint8_t nextHour = (normalizedHours % 12) + 1; // wraps 12→1
                    result.push_back(get_hour_leds_(nextHour));
                }
                else
                {
                    result.push_back(LEDS_NACH);                       // "nach"
                    result.push_back(get_hour_leds_(normalizedHours)); // Current hour
                }
                break;
            case 30:
                // "es ist halb [next hour]"
                result.push_back(LEDS_HALB);                   // "halb"
                result.push_back(get_hour_leds_(displayHour)); // Next hour
                break;
            case 45:
                // "es ist dreiviertel [next hour]"
                if (is_regional_format)
                {
                    result.push_back(LEDS_DREIVIERTEL); // "dreiviertel"
                }
                else
                {
                    result.push_back(LEDS_VIERTEL); // "viertel"
                    result.push_back(LEDS_VOR);     // "vor"
                }
                result.push_back(get_hour_leds_(displayHour)); // Next hour
                break;
            default:
                if (roundedMinutes <= 20)
                {
                    // "es ist [minutes] nach [hour]"
                    result.push_back(get_minute_leds_(roundedMinutes)); // Minutes word
                    result.push_back(LEDS_NACH);                        // "nach"
                    result.push_back(get_hour_leds_(normalizedHours));  // Current hour
                }
                else if (roundedMinutes == 25)
                {
                    // "es ist fuenf vor halb"
                    result.push_back(get_minute_leds_(30 - roundedMinutes)); // Minutes word
                    result.push_back(LEDS_VOR);                              // "vor"
                    result.push_back(LEDS_HALB);                             // "halb"
                }
                else if (roundedMinutes == 35)
                {
                    // "es ist fuenf nach halb"
                    result.push_back(get_minute_leds_(roundedMinutes - 30)); // Minutes word
                    result.push_back(LEDS_NACH);                             // "nach"
                    result.push_back(LEDS_HALB);                             // "halb"
                }
                else
                {
                    // "es ist [minutes] vor [next hour]"
                    result.push_back(get_minute_leds_(60 - roundedMinutes)); // Minutes word
                    result.push_back(LEDS_VOR);                              // "vor"
                }
                result.push_back(get_hour_leds_(displayHour)); // Next hour
                break;
            }

            // Add the option LEDs
            // get the minutes from rounded down minutes to now
            uint8_t minutesToDisplay = minutes - roundedMinutes;
            switch (minutesToDisplay)
            {
            case 1:
                result.push_back(LEDS_OPTION_1);
                break;
            case 2:
                result.push_back(LEDS_OPTION_1);
                result.push_back(LEDS_OPTION_2);
                break;
            case 3:
                result.push_back(LEDS_OPTION_1);
                result.push_back(LEDS_OPTION_2);
                result.push_back(LEDS_OPTION_3);
                break;
            case 4:
                result.push_back(LEDS_OPTION_1);
                result.push_back(LEDS_OPTION_2);
                result.push_back(LEDS_OPTION_3);
                result.push_back(LEDS_OPTION_4);
                break;
            default:
                break;
            }

            return result;
        }

        std::pair<uint16_t, uint16_t> TimeConverterDE::get_hour_leds_(uint8_t hour)
        {
            switch (hour)
            {
            case 1:
                return LEDS_H_EINS;
            case 2:
                return LEDS_H_ZWEI;
            case 3:
                return LEDS_H_DREI;
            case 4:
                return LEDS_H_VIER;
            case 5:
                return LEDS_H_FUENF;
            case 6:
                return LEDS_H_SECHS;
            case 7:
                return LEDS_H_SIEBEN;
            case 8:
                return LEDS_H_ACHT;
            case 9:
                return LEDS_H_NEUN;
            case 10:
                return LEDS_H_ZEHN;
            case 11:
                return LEDS_H_ELF;
            case 12:
                return LEDS_H_ZWOELF;
            default:
                return LEDS_ERROR; // if something goes wrong
            }
        }

        std::pair<uint16_t, uint16_t> TimeConverterDE::get_minute_leds_(uint8_t minutes, bool is_regional_format)
        {
            switch (minutes)
            {
            case 5:
                return LEDS_FUENF;
            case 10:
                return LEDS_ZEHN;
            case 15:
                return LEDS_VIERTEL;
            case 20:
                return LEDS_ZWANZIG;
            case 25:
                return LEDS_FUENF; // "fünf vor halb"
            case 35:
                return LEDS_FUENF; // "fünf nach halb"
            case 40:
                return LEDS_ZWANZIG; // "zwanzig vor"
            case 45:
                return (is_regional_format) ? LEDS_VIERTEL : LEDS_DREIVIERTEL;
            case 50:
                return LEDS_ZEHN; // "zehn vor"
            case 55:
                return LEDS_FUENF; // "fünf vor"
            default:
                return LEDS_ERROR; // if something goes wrong
            }
        }

        std::pair<uint16_t, uint16_t> TimeConverterDE::get_status_leds_(uint8_t status)
        {
            switch (status)
            {
            case 1:
                return LEDS_OPTION_1;
            case 2:
                return LEDS_OPTION_2;
            case 3:
                return LEDS_OPTION_3;
            case 4:
                return LEDS_OPTION_4;
            case 0:
            default:
                return LEDS_ERROR; // if something goes wrong
            }
        }

    }
} // namespace esphome wordclock
