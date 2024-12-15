#include <stdio.h>

#include <pico/stdlib.h>
#include <hardware/pio.h>
#include "distance.pio.h"

#ifdef __cplusplus
extern "C"
{
#endif
    void pio_distance_init(PIO pio, uint sm);
    bool pio_distance_getdistance(PIO pio, uint sm, uint32_t *out_value);
#ifdef __cplusplus
}
#endif