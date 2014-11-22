/**
 * \file
 * \brief SCPI Test:* commands
 */

#include "scpi/scpi.h"
#include "scpi-test.h"
#include "asf.h"
#include "conf_board.h"
#include "synth.h"

scpi_result_t TEST_LEDON (scpi_t * context) {
    int32_t led_number;

    if (!SCPI_ParamInt(context, &led_number, true)) {
        /* Missing parameter error is already registered */
        return SCPI_RES_ERR;
    }

    switch (led_number) {
        case 0:
            gpio_set_pin_low (LED0_GPIO);
            return SCPI_RES_OK;
        case 1:
            gpio_set_pin_low (LED1_GPIO);
            return SCPI_RES_OK;
        default:
            SCPI_ErrorPush (context, SCPI_ERROR_ILLEGAL_PARAMETER_VALUE);
            return SCPI_RES_ERR;
    }
}

scpi_result_t TEST_LEDOFF (scpi_t * context) {
    int32_t led_number;

    if (!SCPI_ParamInt(context, &led_number, true)) {
        /* Missing parameter error is already registered */
        return SCPI_RES_ERR;
    }

    switch (led_number) {
        case 0:
            gpio_set_pin_high (LED0_GPIO);
            return SCPI_RES_OK;
        case 1:
            gpio_set_pin_high (LED1_GPIO);
            return SCPI_RES_OK;
        default:
            SCPI_ErrorPush (context, SCPI_ERROR_ILLEGAL_PARAMETER_VALUE);
            return SCPI_RES_ERR;
    }
}

scpi_result_t TEST_SETNCS (scpi_t *context)
{
    int32_t val;
    if (!SCPI_ParamInt (context, &val, true)) {
        return SCPI_RES_ERR;
    }
    if (val) {
        gpio_set_pin_high (GPIO_SYNTH_nCS);
    } else {
        gpio_set_pin_low (GPIO_SYNTH_nCS);
    }
     return SCPI_RES_OK;   
}

scpi_result_t TEST_SETIOUP (scpi_t *context)
{
    int32_t val;
    if (!SCPI_ParamInt (context, &val, true)) {
        return SCPI_RES_ERR;
    }
    if (val) {
        gpio_set_pin_high (GPIO_SYNTH_IOUPDATE);
    } else {
        gpio_set_pin_low (GPIO_SYNTH_IOUPDATE);
    }
     return SCPI_RES_OK;   
}

scpi_result_t TEST_SETPWRDN (scpi_t *context)
{
    int32_t val;
    if (!SCPI_ParamInt (context, &val, true)) {
        return SCPI_RES_ERR;
    }
    if (val) {
        gpio_set_pin_high (GPIO_SYNTH_PWRDN);
    } else {
        gpio_set_pin_low (GPIO_SYNTH_PWRDN);
    }
     return SCPI_RES_OK;   
}

scpi_result_t TEST_SETMRST (scpi_t *context)
{
    int32_t val;
    if (!SCPI_ParamInt (context, &val, true)) {
        return SCPI_RES_ERR;
    }
    if (val) {
        gpio_set_pin_high (GPIO_SYNTH_MRST);
    } else {
        gpio_set_pin_low (GPIO_SYNTH_MRST);
    }
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
