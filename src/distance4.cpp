#include <pico/stdlib.h>
#include <limits.h>
#include <math.h>
#include <hardware/pio.h>
#include <hardware/clocks.h>
#include "pio/distance.h"
#include "distance4.h"

// pio0 program address
static uint pio0Offset = UINT_MAX;
// pio1 program address
static uint pio1Offset = UINT_MAX;
// next available state machine index
static uint8_t smIndex = 0;

// valid state machines to use
static const uint8_t sm_values[] = {
    USED_STATE_MACHINES};

DistanceSensor4::DistanceSensor4(Config::Distance::Sensor sensor0) : sensorCount(1), sensor0(sensor0), sensor1({0, 0}), sensor2({0, 0}), sensor3({0, 0}),
                                                                     lastDistance0(0), lastDistance1(0), lastDistance2(0), lastDistance3(0)
{
    assert(sensor0.echo == sensor0.trigger + 1); // echo pin must be right after trigger pin
    assert(smIndex < sizeof(sm_values));

    sm0 = sm_values[smIndex++];
    pio_0 = sm0 < 4 ? pio0 : pio1; // pio0 is sm 0 - 3, pio1 is 4 - 6
    sm0 %= 4;                      // sm index relative to pio

    // if creating a new DistanceSensor4 for the first time, load the pio program
    if (pio_0 == pio0 && pio0Offset == UINT_MAX)
    {
        pio0Offset = pio_add_program(pio_0, &distance_program);
    }
    else if (pio_0 == pio1 && pio1Offset == UINT_MAX)
    {
        pio1Offset = pio_add_program(pio_0, &distance_program);
    }

    init0();
}

DistanceSensor4::DistanceSensor4(Config::Distance::Sensor sensor0, Config::Distance::Sensor sensor1) : sensorCount(2), sensor0(sensor0), sensor1(sensor1), sensor2({0, 0}), sensor3({0, 0}),
                                                                                                       lastDistance0(0), lastDistance1(0), lastDistance2(0), lastDistance3(0)
{
    assert(sensor0.echo == sensor0.trigger + 1); // echo pin must be right after trigger pin
    assert(sensor1.echo == sensor1.trigger + 1); // echo pin must be right after trigger pin
    assert(smIndex < sizeof(sm_values));

    sm0 = sm_values[smIndex++];
    pio_0 = sm0 < 4 ? pio0 : pio1; // pio0 is sm 0 - 3, pio1 is 4 - 6
    sm0 %= 4;                      // sm index relative to pio

    // if creating a new DistanceSensor4 for the first time, load the pio program
    if (pio_0 == pio0 && pio0Offset == UINT_MAX)
    {
        pio0Offset = pio_add_program(pio_0, &distance_program);
    }
    else if (pio_0 == pio1 && pio1Offset == UINT_MAX)
    {
        pio1Offset = pio_add_program(pio_0, &distance_program);
    }

    assert(smIndex < sizeof(sm_values));

    sm1 = sm_values[smIndex++];
    pio_1 = sm1 < 4 ? pio0 : pio1; // pio0 is sm 0 - 3, pio1 is 4 - 6
    sm1 %= 4;                      // sm index relative to pio

    // if creating a new DistanceSensor4 for the first time, load the pio program
    if (pio_1 == pio0 && pio0Offset == UINT_MAX)
    {
        pio0Offset = pio_add_program(pio_1, &distance_program);
    }
    else if (pio_1 == pio1 && pio1Offset == UINT_MAX)
    {
        pio1Offset = pio_add_program(pio_1, &distance_program);
    }

    init0();
    init1();
}

DistanceSensor4::DistanceSensor4(Config::Distance::Sensor sensor0, Config::Distance::Sensor sensor1, Config::Distance::Sensor sensor2) : sensorCount(3), sensor0(sensor0), sensor1(sensor1), sensor2(sensor2), sensor3({0, 0}),
                                                                                                                                         lastDistance0(0), lastDistance1(0), lastDistance2(0), lastDistance3(0)
{
    assert(sensor0.echo == sensor0.trigger + 1); // echo pin must be right after trigger pin
    assert(sensor1.echo == sensor1.trigger + 1); // echo pin must be right after trigger pin
    assert(sensor2.echo == sensor2.trigger + 1); // echo pin must be right after trigger pin
    assert(smIndex < sizeof(sm_values));

    sm0 = sm_values[smIndex++];
    pio_0 = sm0 < 4 ? pio0 : pio1; // pio0 is sm 0 - 3, pio1 is 4 - 6
    sm0 %= 4;                      // sm index relative to pio

    // if creating a new DistanceSensor4 for the first time, load the pio program
    if (pio_0 == pio0 && pio0Offset == UINT_MAX)
    {
        pio0Offset = pio_add_program(pio_0, &distance_program);
    }
    else if (pio_0 == pio1 && pio1Offset == UINT_MAX)
    {
        pio1Offset = pio_add_program(pio_0, &distance_program);
    }

    assert(smIndex < sizeof(sm_values));

    sm1 = sm_values[smIndex++];
    pio_1 = sm1 < 4 ? pio0 : pio1; // pio0 is sm 0 - 3, pio1 is 4 - 6
    sm1 %= 4;                      // sm index relative to pio

    // if creating a new DistanceSensor4 for the first time, load the pio program
    if (pio_1 == pio0 && pio0Offset == UINT_MAX)
    {
        pio0Offset = pio_add_program(pio_1, &distance_program);
    }
    else if (pio_1 == pio1 && pio1Offset == UINT_MAX)
    {
        pio1Offset = pio_add_program(pio_1, &distance_program);
    }

    assert(smIndex < sizeof(sm_values));

    sm2 = sm_values[smIndex++];
    pio_2 = sm2 < 4 ? pio0 : pio1; // pio0 is sm 0 - 3, pio1 is 4 - 6
    sm2 %= 4;                      // sm index relative to pio

    // if creating a new DistanceSensor4 for the first time, load the pio program
    if (pio_2 == pio0 && pio0Offset == UINT_MAX)
    {
        pio0Offset = pio_add_program(pio_2, &distance_program);
    }
    else if (pio_2 == pio1 && pio1Offset == UINT_MAX)
    {
        pio1Offset = pio_add_program(pio_2, &distance_program);
    }

    init0();
    init1();
    init2();
}

DistanceSensor4::DistanceSensor4(Config::Distance::Sensor sensor0, Config::Distance::Sensor sensor1, Config::Distance::Sensor sensor2, Config::Distance::Sensor sensor3) : sensorCount(4), sensor0(sensor0), sensor1(sensor1), sensor2(sensor2), sensor3(sensor3),
                                                                                                                                                                           lastDistance0(0), lastDistance1(0), lastDistance2(0), lastDistance3(0)
{
    assert(sensor0.echo == sensor0.trigger + 1); // echo pin must be right after trigger pin
    assert(sensor1.echo == sensor1.trigger + 1); // echo pin must be right after trigger pin
    assert(sensor2.echo == sensor2.trigger + 1); // echo pin must be right after trigger pin
    assert(sensor3.echo == sensor3.trigger + 1); // echo pin must be right after trigger pin
    assert(smIndex < sizeof(sm_values));

    sm0 = sm_values[smIndex++];
    pio_0 = sm0 < 4 ? pio0 : pio1; // pio0 is sm 0 - 3, pio1 is 4 - 6
    sm0 %= 4;                      // sm index relative to pio

    // if creating a new DistanceSensor4 for the first time, load the pio program
    if (pio_0 == pio0 && pio0Offset == UINT_MAX)
    {
        pio0Offset = pio_add_program(pio_0, &distance_program);
    }
    else if (pio_0 == pio1 && pio1Offset == UINT_MAX)
    {
        pio1Offset = pio_add_program(pio_0, &distance_program);
    }

    assert(smIndex < sizeof(sm_values));

    sm1 = sm_values[smIndex++];
    pio_1 = sm1 < 4 ? pio0 : pio1; // pio0 is sm 0 - 3, pio1 is 4 - 6
    sm1 %= 4;                      // sm index relative to pio

    // if creating a new DistanceSensor4 for the first time, load the pio program
    if (pio_1 == pio0 && pio0Offset == UINT_MAX)
    {
        pio0Offset = pio_add_program(pio_1, &distance_program);
    }
    else if (pio_1 == pio1 && pio1Offset == UINT_MAX)
    {
        pio1Offset = pio_add_program(pio_1, &distance_program);
    }

    assert(smIndex < sizeof(sm_values));

    sm2 = sm_values[smIndex++];
    pio_2 = sm2 < 4 ? pio0 : pio1; // pio0 is sm 0 - 3, pio1 is 4 - 6
    sm2 %= 4;                      // sm index relative to pio

    // if creating a new DistanceSensor4 for the first time, load the pio program
    if (pio_2 == pio0 && pio0Offset == UINT_MAX)
    {
        pio0Offset = pio_add_program(pio_2, &distance_program);
    }
    else if (pio_2 == pio1 && pio1Offset == UINT_MAX)
    {
        pio1Offset = pio_add_program(pio_2, &distance_program);
    }

    assert(smIndex < sizeof(sm_values));

    sm3 = sm_values[smIndex++];
    pio_3 = sm3 < 4 ? pio0 : pio1; // pio0 is sm 0 - 3, pio1 is 4 - 6
    sm3 %= 4;                      // sm index relative to pio

    // if creating a new DistanceSensor4 for the first time, load the pio program
    if (pio_3 == pio0 && pio0Offset == UINT_MAX)
    {
        pio0Offset = pio_add_program(pio_3, &distance_program);
    }
    else if (pio_3 == pio1 && pio1Offset == UINT_MAX)
    {
        pio1Offset = pio_add_program(pio_3, &distance_program);
    }

    init0();
    init1();
    init2();
    init3();
}

void DistanceSensor4::init0()
{
    // init pio program with trigger pin
    distance_program_init(pio_0, sm0, pio_0 == pio0 ? pio0Offset : pio1Offset, sensor0.trigger);
    pio_distance_init(pio_0, sm0);
    pio_sm_set_clkdiv(pio_0, sm0, 1.f);
}

void DistanceSensor4::init1()
{
    // init pio program with trigger pin
    distance_program_init(pio_1, sm1, pio_1 == pio0 ? pio0Offset : pio1Offset, sensor1.trigger);
    pio_distance_init(pio_1, sm1);
    pio_sm_set_clkdiv(pio_1, sm1, 1.f);
}

void DistanceSensor4::init2()
{
    // init pio program with trigger pin
    distance_program_init(pio_2, sm2, pio_2 == pio0 ? pio0Offset : pio1Offset, sensor2.trigger);
    pio_distance_init(pio_2, sm2);
    pio_sm_set_clkdiv(pio_2, sm2, 1.f);
}

void DistanceSensor4::init3()
{
    // init pio program with trigger pin
    distance_program_init(pio_3, sm3, pio_3 == pio0 ? pio0Offset : pio1Offset, sensor3.trigger);
    pio_distance_init(pio_3, sm3);
    pio_sm_set_clkdiv(pio_3, sm3, 1.f);
}

DistanceSensor4::~DistanceSensor4()
{
    if (sensorCount > 0)
    {
        gpio_deinit(sensor0.trigger);
        gpio_deinit(sensor0.echo);
    }
    if (sensorCount > 1)
    {
        gpio_deinit(sensor1.trigger);
        gpio_deinit(sensor1.echo);
    }
    if (sensorCount > 2)
    {
        gpio_deinit(sensor2.trigger);
        gpio_deinit(sensor2.echo);
    }
    if (sensorCount > 3)
    {
        gpio_deinit(sensor3.trigger);
        gpio_deinit(sensor3.echo);
    }
}

std::tuple<float, float, float, float> DistanceSensor4::getDistance()
{
    uint32_t cycles0 = 0;
    uint32_t cycles1 = 0;
    uint32_t cycles2 = 0;
    uint32_t cycles3 = 0;

    uint32_t clk = clock_get_hz(clk_sys);
    uint32_t ns_per_clk = 1000000000UL / clk;
    uint32_t max_duration_ns = distance_MAX_ECHO_TIME * distance_ECHO_LOOP_CYCLES * ns_per_clk;
    int64_t timeout = 2 * max_duration_ns / 1000;

    // initiate trigger
    if (sensorCount > 0)
    {
        pio_sm_put_blocking(pio_0, sm0, distance_TRIGGER_PULSE_LENGTH); // write TRIGGER_PULSE_LENGTH
        check_pio_param(pio_0);
        check_sm_param(sm0);
    }
    if (sensorCount > 1)
    {
        pio_sm_put_blocking(pio_1, sm1, distance_TRIGGER_PULSE_LENGTH); // write TRIGGER_PULSE_LENGTH
        check_pio_param(pio_1);
        check_sm_param(sm1);
    }
    if (sensorCount > 2)
    {
        pio_sm_put_blocking(pio_2, sm2, distance_TRIGGER_PULSE_LENGTH); // write TRIGGER_PULSE_LENGTH
        check_pio_param(pio_2);
        check_sm_param(sm2);
    }
    if (sensorCount > 3)
    {
        pio_sm_put_blocking(pio_3, sm3, distance_TRIGGER_PULSE_LENGTH); // write TRIGGER_PULSE_LENGTH
        check_pio_param(pio_3);
        check_sm_param(sm3);
    }

    // set got values true if not used (to escape loop)
    bool got0 = sensorCount <= 0;
    bool got1 = sensorCount <= 1;
    bool got2 = sensorCount <= 2;
    bool got3 = sensorCount <= 3;

    absolute_time_t start = get_absolute_time();

    while (!got0 || !got1 || !got2 || !got3 /* loop until all distances read */)
    {
        tight_loop_contents();
        if (absolute_time_diff_us(start, get_absolute_time()) > timeout) // same timeout - no matter which sensor
        {
            break;
        }

        if (!got0 && !pio_sm_is_rx_fifo_empty(pio_0, sm0))
        {
            got0 = true;
            cycles0 = pio_sm_get(pio_0, sm0);
        }

        if (!got1 && !pio_sm_is_rx_fifo_empty(pio_1, sm1))
        {
            got1 = true;
            cycles1 = pio_sm_get(pio_1, sm1);
        }

        if (!got2 && !pio_sm_is_rx_fifo_empty(pio_2, sm2))
        {
            got2 = true;
            cycles2 = pio_sm_get(pio_2, sm2);
        }

        if (!got3 && !pio_sm_is_rx_fifo_empty(pio_3, sm3))
        {
            got3 = true;
            cycles3 = pio_sm_get(pio_3, sm3);
        }
    }

    if (got0)
    {
        uint32_t duration_ns = (distance_MAX_ECHO_TIME - cycles0) * distance_ECHO_LOOP_CYCLES * ns_per_clk;
        lastDistance0 = duration_ns * 0.000034f / 2;
    }
    else
    {
        // If timed out, restart state machine (probably frozen due to disconnected sensor)
        init0();
    }

    if (got1)
    {
        uint32_t duration_ns = (distance_MAX_ECHO_TIME - cycles1) * distance_ECHO_LOOP_CYCLES * ns_per_clk;
        lastDistance1 = duration_ns * 0.000034f / 2;
    }
    else
    {
        // If timed out, restart state machine (probably frozen due to disconnected sensor)
        init1();
    }

    if (got2)
    {
        uint32_t duration_ns = (distance_MAX_ECHO_TIME - cycles2) * distance_ECHO_LOOP_CYCLES * ns_per_clk;
        lastDistance2 = duration_ns * 0.000034f / 2;
    }
    else
    {
        // If timed out, restart state machine (probably frozen due to disconnected sensor)
        init2();
    }

    if (got3)
    {
        uint32_t duration_ns = (distance_MAX_ECHO_TIME - cycles3) * distance_ECHO_LOOP_CYCLES * ns_per_clk;
        lastDistance3 = duration_ns * 0.000034f / 2;
    }
    else
    {
        // If timed out, restart state machine (probably frozen due to disconnected sensor)
        init3();
    }

    // return either last distance (if failed) or current distance
    return {lastDistance0, lastDistance1, lastDistance2, lastDistance3};
}