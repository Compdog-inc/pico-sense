#ifndef _DISTANCE_SENSOR_H
#define _DISTANCE_SENSOR_H

#include <stdlib.h>
#include <hardware/pio.h>
#include <hardware/clocks.h>
#include "config/options.h"

#define USED_STATE_MACHINES 0, 1, 2, 3, 5, 6

class DistanceSensor4
{
public:
    DistanceSensor4(Config::Distance::Sensor sensor0);
    DistanceSensor4(Config::Distance::Sensor sensor0, Config::Distance::Sensor sensor1);
    DistanceSensor4(Config::Distance::Sensor sensor0, Config::Distance::Sensor sensor1, Config::Distance::Sensor sensor2);
    DistanceSensor4(Config::Distance::Sensor sensor0, Config::Distance::Sensor sensor1, Config::Distance::Sensor sensor2, Config::Distance::Sensor sensor3);
    ~DistanceSensor4();

    std::tuple<float, float, float, float> getDistance();

private:
    void init0();
    void init1();
    void init2();
    void init3();

    PIO pio_0;
    PIO pio_1;
    PIO pio_2;
    PIO pio_3;
    uint sm0;
    uint sm1;
    uint sm2;
    uint sm3;

    uint8_t sensorCount;

    Config::Distance::Sensor sensor0;
    Config::Distance::Sensor sensor1;
    Config::Distance::Sensor sensor2;
    Config::Distance::Sensor sensor3;
    float lastDistance0;
    float lastDistance1;
    float lastDistance2;
    float lastDistance3;
};

#endif