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
    scpi_number_t led_number;

    if (!SCPI_ParamNumber(context, &led_number, true)) {
        /* Missing parameter error is already registered */
        return SCPI_RES_ERR;
    }

    unsigned lednum = led_number.value;

    switch (lednum) {
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
    scpi_number_t led_number;

    if (!SCPI_ParamNumber(context, &led_number, false)) {
        /* Missing parameter error is already registered */
        return SCPI_RES_ERR;
    }

    unsigned lednum = led_number.value;

    switch (lednum) {
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
    scpi_number_t Sval;
    if (!SCPI_ParamNumber (context, &Sval, false)) {
        return SCPI_RES_ERR;
    }
    unsigned val = Sval.value;
    if (val) {
        gpio_set_pin_high (GPIO_SYNTH_nCS);
    } else {
        gpio_set_pin_low (GPIO_SYNTH_nCS);
    }
     return SCPI_RES_OK;   
}

scpi_result_t TEST_SETIOUP (scpi_t *context)
{
    scpi_number_t Sval;
    if (!SCPI_ParamNumber (context, &Sval, false)) {
        return SCPI_RES_ERR;
    }
    unsigned val = Sval.value;
    if (val) {
        gpio_set_pin_high (GPIO_SYNTH_IOUPDATE);
    } else {
        gpio_set_pin_low (GPIO_SYNTH_IOUPDATE);
    }
     return SCPI_RES_OK;   
}

scpi_result_t TEST_SETPWRDN (scpi_t *context)
{
    scpi_number_t Sval;
    if (!SCPI_ParamNumber (context, &Sval, false)) {
        return SCPI_RES_ERR;
    }
    unsigned val = Sval.value;
    if (val) {
        gpio_set_pin_high (GPIO_SYNTH_PWRDN);
    } else {
        gpio_set_pin_low (GPIO_SYNTH_PWRDN);
    }
     return SCPI_RES_OK;   
}

scpi_result_t TEST_SETMRST (scpi_t *context)
{
    scpi_number_t Sval;
    if (!SCPI_ParamNumber (context, &Sval, false)) {
        return SCPI_RES_ERR;
    }
    unsigned val = Sval.value;
    if (val) {
        gpio_set_pin_high (GPIO_SYNTH_MRST);
    } else {
        gpio_set_pin_low (GPIO_SYNTH_MRST);
    }
     return SCPI_RES_OK;   
}

scpi_result_t TEST_SPI (scpi_t *context)
{
    scpi_number_t Sval;
    if (!SCPI_ParamNumber (context, &Sval, false)) {
        return SCPI_RES_ERR;
    }
    uint8_t val = Sval.value;
    printf ("Transmitting value %02x\r\n", val);
    spi_write (SPI_MASTER_BASE, val, 0, 0);
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
    scpi_number_t SNchannel, SNfreq;
    if (!SCPI_ParamNumber (context, &SNchannel, false)) {
        return SCPI_RES_ERR;
    }
    if (!SCPI_ParamNumber (context, &SNfreq, false)) {
        return SCPI_RES_ERR;
    }
    unsigned ch = SNchannel.value;
    printf ("Setting frequency %d to %f\r\n", ch, SNfreq.value);
    synth_set_frequency (ch, SNfreq.value);
    return SCPI_RES_OK;   
}
