#include <pico/stdlib.h>
#include <limits.h>
#include <math.h>
#include <hardware/pio.h>
#include <hardware/clocks.h>
#include "pio/distance.h"
#include "distance.h"

// pio0 program address
static uint pio0Offset = UINT_MAX;
// pio1 program address
static uint pio1Offset = UINT_MAX;
// next available state machine index
static uint8_t smIndex = 0;

// valid state machines to use
static const uint8_t sm_values[] = {
    USED_STATE_MACHINES};

DistanceSensor::DistanceSensor(Config::Distance::Sensor sensor) : sensor(sensor), lastDistance(0)
{
    assert(sensor.echo == sensor.trigger + 1); // echo pin must be right after trigger pin
    assert(smIndex < sizeof(sm_values));

    sm = sm_values[smIndex++];
    pio = sm < 4 ? pio0 : pio1; // pio0 is sm 0 - 3, pio1 is 4 - 6
    sm %= 4;                    // sm index relative to pio

    // if creating a new Motor for the first time, load the pio program
    if (pio == pio0 && pio0Offset == UINT_MAX)
    {
        pio0Offset = pio_add_program(pio, &distance_program);
    }
    else if (pio == pio1 && pio1Offset == UINT_MAX)
    {
        pio1Offset = pio_add_program(pio, &distance_program);
    }

    init();
}

void DistanceSensor::init()
{
    // init pio program with trigger pin
    distance_program_init(pio, sm, pio == pio0 ? pio0Offset : pio1Offset, sensor.trigger);
    pio_distance_init(pio, sm);
    pio_sm_set_clkdiv(pio, sm, 1.f);
}

DistanceSensor::~DistanceSensor()
{
    gpio_deinit(sensor.trigger);
    gpio_deinit(sensor.echo);
}

float DistanceSensor::getDistance()
{
    uint32_t cycles;

    if (pio_distance_getdistance(pio, sm, &cycles))
    {
        uint32_t clk = clock_get_hz(clk_sys);
        uint32_t ns_per_clk = 1000000000UL / clk;
        uint32_t duration_ns = (distance_MAX_ECHO_TIME - cycles) * distance_ECHO_LOOP_CYCLES * ns_per_clk;
        lastDistance = duration_ns * 0.000034f / 2;
    }
    else
    {
        // If timed out, restart state machine (probably frozen due to disconnected sensor)
        init();
    }

    // return either last distance (if failed) or current distance
    return lastDistance;
}