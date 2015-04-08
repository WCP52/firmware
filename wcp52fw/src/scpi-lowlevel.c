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
#   define XPINGROUP(grp)
#   define XPIN(name, pin, flags, descr) \
    if (!found_pin && !strcmp (#name, buffer)) { \
        found_pin = true; \
        pin_action(PIO_ ## pin ## _IDX, (flags), buffer); \
    }
    PIN_LIST
#   undef XPINGROUP
#   undef XPIN

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

#   define XPINGROUP(grp)  printf("== %s ==\r\n", (grp));
#   define XPIN(name, pin, flags, descr)  printf("%-20s %-30s %s\r\n", #name, #flags, (descr));
    PIN_LIST
#   undef XPINGROUP
#   undef XPIN

    return SCPI_RES_OK;
}
