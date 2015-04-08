/**
 * @file
 * SCPI LOWlevel:* commands
 */

// Atmel ASF includes
#include <pio.h>
#include <string.h>

#include "scpi/scpi.h"
#include "scpi-lowlevel.h"
#include "conf_board.h"
#include "util.h"

/**
 * SCPI pin action: set
 */
scpi_result_t pin_action_set (uint32_t pin, uint32_t flags, const char *name)
{
    if ((flags & PIO_TYPE_Msk) != PIO_OUTPUT_0 && (flags & PIO_TYPE_Msk) != PIO_OUTPUT_1) {
        printf ("Pin '%s' is not an output!\r\n", name);
        return SCPI_RES_ERR;
    }

    pio_set_pin_high(pin);
    return SCPI_RES_OK;
}

/**
 * SCPI pin action: clr
 */
scpi_result_t pin_action_clr (uint32_t pin, uint32_t flags, const char *name)
{
    if ((flags & PIO_TYPE_Msk) != PIO_OUTPUT_0 && (flags & PIO_TYPE_Msk) != PIO_OUTPUT_1) {
        printf ("Pin '%s' is not an output!\r\n", name);
        return SCPI_RES_ERR;
    }

    pio_set_pin_low(pin);
    return SCPI_RES_OK;
}

/**
 * SCPI pin action: get
 */
scpi_result_t pin_action_get (uint32_t pin, uint32_t flags, const char *name)
{
    (void) flags;
    (void) name;
    printf ("%d\r\n", pio_get_pin_value(pin) ? 1 : 0);
    return SCPI_RES_OK;
}

/**
 * SCPI: low-level pin actions
 * LOWlevel:SETpin NAME
 * LOWlevel:CLRpin NAME
 * LOWlevel:GETpin NAME
 *
 * @param context - active SCPI context
 * @return success or failure
 */
scpi_result_t LOWLEVEL_PIN_ACTION (scpi_t *context)
{
    char const *str;
    size_t len;
    scpi_result_t (*pin_action) (uint32_t, uint32_t, const char *);

    if (!SCPI_ParamString(context, &str, &len, true)) {
        return SCPI_RES_ERR;
    }

    if (SCPI_IsCmd(context, "LOWlevel:SETpin")) {
        pin_action = &pin_action_set;
    } else if (SCPI_IsCmd(context, "LOWLEVEL:CLRpin")) {
        pin_action = &pin_action_clr;
    } else {
        pin_action = &pin_action_get;
    }

#define buflen 32
    char buffer[buflen];
    if (len > buflen - 1) {
        puts ("String too long\r");
        return SCPI_RES_ERR;
    }

    memcpy(buffer, str, len);
    buffer[len] = 0;

    bool found_pin = false;

    bool each_pin_action (const struct pin_info *pin) {
        if (!strcmp (pin->pin_name_str, buffer)) {
            pin_action(pin->index, pin->flags, buffer);
            found_pin = true;
            return false; // break
        } else {
            return true; // continue
        }
    }

    for_each_pin (&each_pin_action, NULL);

    if (!found_pin) {
        printf ("Pin '%s' not found!\r\n", buffer);
        return SCPI_RES_ERR;
    } else {
        return SCPI_RES_OK;
    }
}

scpi_result_t LOWLEVEL_LIST_PINS (scpi_t *context)
{
    (void) context;

    bool print_pin_group (const char *text) {
        printf ("==== %s ====\r\n", text);
        return true;
    }

    bool print_pin (const struct pin_info *pin) {
        printf ("%-20s %-30s %s\r\n", pin->pin_name_str, pin->flags_str, pin->description);
        return true;
    }

    for_each_pin (&print_pin, &print_pin_group);

    return SCPI_RES_OK;
}
