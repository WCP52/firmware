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

scpi_result_t TEST_LED (scpi_t *context) {
    int32_t led_number;

    if (!SCPI_ParamInt(context, &led_number, true)) {
        /* Missing parameter error is already registered */
        return SCPI_RES_ERR;
    }

    int value = ! SCPI_CmdId (context);

    switch (led_number) {
        case 0:
            util_set_pin (LED0_GPIO, value);
            return SCPI_RES_OK;
        case 1:
            util_set_pin (LED1_GPIO, value);
            return SCPI_RES_OK;
        default:
            SCPI_ErrorPush (context, SCPI_ERROR_ILLEGAL_PARAMETER_VALUE);
            return SCPI_RES_ERR;
    }
}

scpi_result_t TEST_SET (scpi_t *context)
{
    int32_t val;
    if (!SCPI_ParamInt (context, &val, true)) {
        return SCPI_RES_ERR;
    }

    /* Determine which command was called */
    int32_t cmdid = SCPI_CmdId (context);
    int32_t pin;
    switch (cmdid) {
        case 0:
            pin = GPIO_SYNTH_nCS;
            break;
        case 1:
            pin = GPIO_SYNTH_IOUPDATE;
            break;
        case 2:
            pin = GPIO_SYNTH_PWRDN;
            break;
        case 3:
            pin = GPIO_SYNTH_MRST;
            break;
        default:
            pin = LED0_GPIO;
    }

    util_set_pin (pin, val);
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
