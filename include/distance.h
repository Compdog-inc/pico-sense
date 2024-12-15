#ifndef _DISTANCE_SENSOR_H
#define _DISTANCE_SENSOR_H

#include <stdlib.h>
#include <hardware/pio.h>
#include <hardware/clocks.h>
#include "config/options.h"

#define USED_STATE_MACHINES 0, 1, 2, 3, 5, 6

class DistanceSensor
{
public:
    DistanceSensor(Config::Distance::Sensor sensor);
    ~DistanceSensor();

    float getDistance();

private:
    void init();

    PIO pio;
    uint sm;
    Config::Distance::Sensor sensor;
    float lastDistance;
};

#endif