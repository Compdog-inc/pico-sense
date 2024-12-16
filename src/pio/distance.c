#include <stdio.h>

#include <pico/stdlib.h>
#include <hardware/pio.h>
#include "distance.pio.h"
#include <hardware/clocks.h>

#include "distance.h"

void pio_distance_init(PIO pio, uint sm)
{
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_put_blocking(pio, sm, distance_MAX_ECHO_TIME); // write MAX_ECHO_TIME
    pio_sm_set_enabled(pio, sm, true);
}