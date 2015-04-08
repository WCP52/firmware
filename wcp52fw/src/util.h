/**
 * \file
 * Miscellaneous utilities
 */

#ifndef _WCP52_UTIL_H
#define _WCP52_UTIL_H

#include <gpio.h>

/**
 * Set a pin to a given value.
 * \param pin   GPIO pin ID.
 * \param value 0 for low, nonzero for high.
 */
static inline void util_set_pin(uint32_t pin, int_fast8_t value)
{
    if (value) {
        gpio_set_pin_high(pin);
    } else {
        gpio_set_pin_low(pin);
    }
}

#endif /* _WCP52_UTIL_H */
