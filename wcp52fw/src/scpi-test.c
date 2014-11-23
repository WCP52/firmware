/**
 * \file
 * \brief SCPI Test:* commands
 */

#include "scpi/scpi.h"
#include "scpi-test.h"
#include "asf.h"
#include "conf_board.h"
#include "synth.h"
#include "util.h"

/* List of pins we can set or clear */
static const char *SETCLR_NAMES[] = {
    "NCS", "IOUPdate", "PWRDN", "MRST", "LED0", "LED1", "LIST", NULL
};

/* Polarity of each one (1 for invert, 2 for "this is LIST") */
static const uint8_t SETCLR_POL[] = {
    0, 0, 0, 0, 1, 1, 2 };

/* List of the actual pins */
static const int32_t SETCLR_PINS[] = {
    GPIO_SYNTH_nCS, GPIO_SYNTH_IOUPDATE, GPIO_SYNTH_PWRDN,
    GPIO_SYNTH_MRST, LED0_GPIO, LED1_GPIO };

scpi_result_t TEST_SETCLR (scpi_t *context)
{
    int32_t choice;

    if (!SCPI_ParamChoice (context, SETCLR_NAMES, &choice, true)) {
        return SCPI_RES_ERR;
    }

    /* List names */
    if (SETCLR_POL[choice] == 2) {
        /* List! */
        size_t i;
        for (i = 0; SETCLR_NAMES[i]; ++i) {
            if (SETCLR_POL[i] != 2) {
                if (i) {
                    fputs (", ", stdout);
                }
                fputs (SETCLR_NAMES[i], stdout);
            }
        }
        fputs ("\r\n", stdout);
        return SCPI_RES_OK;
    }

    uint8_t value = SCPI_IsCmd (context, "TEST:SET") ? 1 : 0;
    value ^= SETCLR_POL[choice];

    util_set_pin (SETCLR_PINS[choice], value);
    return SCPI_RES_OK;   
}

scpi_result_t TEST_SPI (scpi_t *context)
{
    int32_t val;
    if (!SCPI_ParamInt (context, &val, true)) {
        return SCPI_RES_ERR;
    }
    uint8_t val_byte = (uint8_t) val;
    printf ("Transmitting value %02x\r\n", val_byte);
    spi_write (SPI_MASTER_BASE, val_byte, 0, 0);
    return SCPI_RES_OK;   
}

scpi_result_t TEST_INIF (scpi_t *context)
{
    (void) context;
    synth_initialize_interface ();
    return SCPI_RES_OK;   
}

scpi_result_t TEST_INCK (scpi_t *context)
{
    (void) context;
    synth_initialize_clock ();
    return SCPI_RES_OK;   
}

scpi_result_t TEST_FREQ (scpi_t *context)
{
    int32_t ch;
    scpi_number_t freq;
    if (!SCPI_ParamInt (context, &ch, true)) {
        return SCPI_RES_ERR;
    }
    if (!SCPI_ParamNumber (context, &freq, true)) {
        return SCPI_RES_ERR;
    }
    unsigned ch_uns = (unsigned) ch;
    printf ("Setting frequency %d to %f\r\n", ch_uns, freq.value);
    synth_set_frequency (ch_uns, freq.value);
    return SCPI_RES_OK;   
}
