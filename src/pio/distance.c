#include <stdio.h>

#include <pico/stdlib.h>
#include <hardware/pio.h>
#include "distance.pio.h"
#include <hardware/clocks.h>

#include "distance.h"

/*! \brief Read a word of data from a state machine's RX FIFO, blocking if the FIFO is empty
 *  \ingroup hardware_pio
 *
 * \param pio The PIO instance; e.g. \ref pio0 or \ref pio1
 * \param sm State machine index (0..3)
 * \param timeout Timeout in microseconds or -1 for infinite
 * \param out_value Pointer to where the read value should be stored
 *
 * \return true if a word was read, false if the timeout expired
 */
static inline bool pio_sm_get_blocking_tmo(PIO pio, uint sm, int64_t timeout, uint32_t *out_value)
{
    check_pio_param(pio);
    check_sm_param(sm);
    absolute_time_t start = get_absolute_time();
    while (pio_sm_is_rx_fifo_empty(pio, sm))
    {
        tight_loop_contents();
        if (timeout != -1 && absolute_time_diff_us(start, get_absolute_time()) > timeout)
        {
            return false;
        }
    }
    *out_value = pio_sm_get(pio, sm);
    return true;
}

void pio_distance_init(PIO pio, uint sm)
{
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_put_blocking(pio, sm, distance_MAX_ECHO_TIME); // write MAX_ECHO_TIME
    pio_sm_set_enabled(pio, sm, true);
}

bool pio_distance_getdistance(PIO pio, uint sm, uint32_t *out_value)
{
    uint32_t clk = clock_get_hz(clk_sys);
    uint32_t ns_per_clk = 1000000000UL / clk;
    uint32_t max_duration_ns = distance_MAX_ECHO_TIME * distance_ECHO_LOOP_CYCLES * ns_per_clk;

    pio_sm_put_blocking(pio, sm, distance_TRIGGER_PULSE_LENGTH); // write TRIGGER_PULSE_LENGTH
    return pio_sm_get_blocking_tmo(pio, sm, 2 * max_duration_ns / 1000, out_value);
}