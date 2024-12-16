#include <stdio.h>

#include <pico/stdlib.h>
#include <hardware/pio.h>
#include "distance.pio.h"

#ifdef __cplusplus
extern "C"
{
#endif
    void pio_distance_init(PIO pio, uint sm);
#ifdef __cplusplus
}
#endif