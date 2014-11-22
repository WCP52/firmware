/*-
 * Copyright (c) 2012-2013 Jan Breuer,
 *
 * All Rights Reserved
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file   scpi-def.c
 * @date   Thu Nov 15 10:58:45 UTC 2012
 * 
 * @brief  SCPI parser test
 * 
 * 
 */

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scpi/scpi.h"
#include "scpi-def.h"
#include "asf.h"
#include "conf_board.h"
#include "synth.h"

scpi_result_t TEST_LEDON (scpi_t * context);
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

scpi_result_t TEST_LEDOFF (scpi_t * context);
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

scpi_result_t TEST_SETNCS (scpi_t *context);
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

scpi_result_t TEST_SETIOUP (scpi_t *context);
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

scpi_result_t TEST_SETPWRDN (scpi_t *context);
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

scpi_result_t TEST_SETMRST (scpi_t *context);
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

scpi_result_t TEST_SPI (scpi_t *context);
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

scpi_result_t TEST_INIF (scpi_t *context);
scpi_result_t TEST_INIF (scpi_t *context)
{
    (void) context;
    synth_initialize_interface ();
    return SCPI_RES_OK;   
}

scpi_result_t TEST_INCK (scpi_t *context);
scpi_result_t TEST_INCK (scpi_t *context)
{
    (void) context;
    synth_initialize_clock ();
    return SCPI_RES_OK;   
}

scpi_result_t TEST_FREQ (scpi_t *context);
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

static const scpi_command_t scpi_commands[] = {
    /* IEEE Mandated Commands (SCPI std V1999.0 4.1.1) */
    { .pattern = "*CLS", .callback = SCPI_CoreCls,},
    { .pattern = "*ESE", .callback = SCPI_CoreEse,},
    { .pattern = "*ESE?", .callback = SCPI_CoreEseQ,},
    { .pattern = "*ESR?", .callback = SCPI_CoreEsrQ,},
    { .pattern = "*IDN?", .callback = SCPI_CoreIdnQ,},
    { .pattern = "*OPC", .callback = SCPI_CoreOpc,},
    { .pattern = "*OPC?", .callback = SCPI_CoreOpcQ,},
    { .pattern = "*RST", .callback = SCPI_CoreRst,},
    { .pattern = "*SRE", .callback = SCPI_CoreSre,},
    { .pattern = "*SRE?", .callback = SCPI_CoreSreQ,},
    { .pattern = "*STB?", .callback = SCPI_CoreStbQ,},
    { .pattern = "*TST?", .callback = SCPI_CoreTstQ,},
    { .pattern = "*WAI", .callback = SCPI_CoreWai,},

    /* Required SCPI commands (SCPI std V1999.0 4.2.1) */
    {.pattern = "SYSTem:ERRor[:NEXT]?", .callback = SCPI_SystemErrorNextQ,},
    {.pattern = "SYSTem:ERRor:COUNt?", .callback = SCPI_SystemErrorCountQ,},
    {.pattern = "SYSTem:VERSion?", .callback = SCPI_SystemVersionQ,},

    //{.pattern = "STATus:OPERation?", .callback = scpi_stub_callback,},
    //{.pattern = "STATus:OPERation:EVENt?", .callback = scpi_stub_callback,},
    //{.pattern = "STATus:OPERation:CONDition?", .callback = scpi_stub_callback,},
    //{.pattern = "STATus:OPERation:ENABle", .callback = scpi_stub_callback,},
    //{.pattern = "STATus:OPERation:ENABle?", .callback = scpi_stub_callback,},

    {.pattern = "STATus:QUEStionable[:EVENt]?", .callback = SCPI_StatusQuestionableEventQ,},
    //{.pattern = "STATus:QUEStionable:CONDition?", .callback = scpi_stub_callback,},
    {.pattern = "STATus:QUEStionable:ENABle", .callback = SCPI_StatusQuestionableEnable,},
    {.pattern = "STATus:QUEStionable:ENABle?", .callback = SCPI_StatusQuestionableEnableQ,},

    {.pattern = "STATus:PRESet", .callback = SCPI_StatusPreset,},

    /* Test commands */
    {.pattern = "Test:LEDON", .callback = TEST_LEDON,},
    {.pattern = "Test:LEDOFF", .callback = TEST_LEDOFF,},
    {.pattern = "Test:SETNCS", .callback = TEST_SETNCS,},
    {.pattern = "Test:SETIOUP", .callback = TEST_SETIOUP,},
    {.pattern = "Test:SETPWRDN", .callback = TEST_SETPWRDN,},
    {.pattern = "Test:SETMRST", .callback = TEST_SETMRST,},
    {.pattern = "Test:SPI", .callback = TEST_SPI,},
    {.pattern = "Test:INIF", .callback = TEST_INIF,}, /* Init interface */
    {.pattern = "Test:INCK", .callback = TEST_INCK,}, /* Init clock */
    {.pattern = "Test:FREQ", .callback = TEST_FREQ,},

    SCPI_CMD_LIST_END
};

static scpi_interface_t scpi_interface = {
    .error = SCPI_Error,
    .write = SCPI_Write,
    .control = SCPI_Control,
    .flush = SCPI_Flush,
    .reset = SCPI_Reset,
    .test = SCPI_Test,
};

#define SCPI_INPUT_BUFFER_LENGTH 256
static char scpi_input_buffer[SCPI_INPUT_BUFFER_LENGTH];

static scpi_reg_val_t scpi_regs[SCPI_REG_COUNT];


scpi_t scpi_context = {
    .cmdlist = scpi_commands,
    .buffer = {
        .length = SCPI_INPUT_BUFFER_LENGTH,
        .data = scpi_input_buffer,
    },
    .interface = &scpi_interface,
    .registers = scpi_regs,
    .units = scpi_units_def,
    .special_numbers = scpi_special_numbers_def,
    .idn = {"MANUFACTURE", "INSTR2013", NULL, "01-02"},
};
