/**
 * \file
 * Miscellaneous utilities
 */

#ifndef _WCP52_UTIL_H
#define _WCP52_UTIL_H

#include <pio.h>
#include <inttypes.h>
#include <stdbool.h>

/**
 * Set a pin to a given value.
 * \param pin   GPIO pin ID.
 * \param value 0 for low, nonzero for high.
 */
static inline void util_set_pin(uint32_t pin, int_fast8_t value)
{
    if (value) {
        pio_set_pin_high(pin);
    } else {
        pio_set_pin_low(pin);
    }
}

/**
 * Pin info struct that is passed to a function by for_each_pin
 */
struct pin_info {
    const char *pin_name_str;
    const char *index_str;
    const char *flags_str;
    const char *description;

    uint32_t index;
    uint32_t flags;
};

/**
 * Call a function for each GPIO pin and pin group header.
 *
 * Function prototype for pins:
 * bool fxn(const struct pin_info *pin);
 *
 * Function prototype for groups:
 * bool fxn(const char *text);
 *
 * Return 'true' to continue to next pins, or 'false' to break.
 *
 * @param pinfxn - function pointer to call for pins. can be NULL
 * @param grpfxn - function pointer to call for groups. can be NULL
 */
void for_each_pin (
        bool (*pinfxn) (const struct pin_info *pin),
        bool (*grpfxn) (const char *text));

#endif /* _WCP52_UTIL_H */
