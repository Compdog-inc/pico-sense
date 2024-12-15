#ifndef _OPTIONS_H
#define _OPTIONS_H

// Standard headers
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <array>

// Hardware headers
#include <pico/stdlib.h>

namespace Config
{
    namespace Lights
    {
        static constexpr uint PULSE_LENGTH = 2000000;
        static constexpr uint PULSE_HALF_LENGTH = 1000000;
        static constexpr uint PULSE_DIVISOR = 16;

        static constexpr uint BLINK_LENGTH = 1000000;
        static constexpr uint BLINK_ON_LENGTH = 250000;
    }

    namespace Distance
    {
        struct Sensor
        {
            uint trigger;
            uint echo;
        };

        static constexpr Sensor SENSOR_0 = {6, 7};
        static constexpr Sensor SENSOR_5 = {8, 9};
        static constexpr Sensor SENSOR_1 = {10, 11};
        static constexpr Sensor SENSOR_4 = {12, 13};
        static constexpr Sensor SENSOR_2 = {14, 15};
        static constexpr Sensor SENSOR_3 = {16, 17};
    }
}

#endif