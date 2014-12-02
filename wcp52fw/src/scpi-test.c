/**
 * \file
 * \brief SCPI Test:* commands
 */

#include "scpi/scpi.h"
#include "scpi-test.h"
#include "asf.h"
#include "conf_board.h"
#include "synth.h"
#include "acquisition.h"
#include "util.h"

/************************************
 * Code for TEST:SET and TEST:CLEAR *
 ************************************/

// List of pins we can set or clear
static const char *SETCLR_NAMES[] = {
    "NCS", "IOUPdate", "PWRDN", "MRST", "SYNCIO", "LED0", "LED1", "LIST", NULL
};

// Polarity of each pin (1 for inverted; 2 indicates the LIST item to avoid
// a strcmp
static const uint8_t SETCLR_POL[] = {
    0, 0, 0, 0, 0, 1, 1, 2 };

// The GPIO pin IDs
static const int32_t SETCLR_PINS[] = {
    GPIO_SYNTH_nCS, GPIO_SYNTH_IOUPDATE, GPIO_SYNTH_PWRDN,
    GPIO_SYNTH_MRST, GPIO_SYNTH_SYNCIO, LED0_GPIO, LED1_GPIO };

/**
 * SCPI: Set or clear a pin.
 * Test:Set PIN_ID      Test:Set LIST (to list all)
 * Test:Clear PIN_ID    Test:Clear LIST (to list all)
 * 
 * \param context   Active SCPI context
 * \return  Success or failure
 */
scpi_result_t TEST_SETCLR(scpi_t *context)
{
    int32_t choice;

    if (!SCPI_ParamChoice(context, SETCLR_NAMES, &choice, true)) {
        return SCPI_RES_ERR;
    }

    // List names
    if (SETCLR_POL[choice] == 2) {
        size_t i;
        for (i = 0; SETCLR_NAMES[i]; ++i) {
            if (SETCLR_POL[i] != 2) {
                if (i) {
                    fputs(", ", stdout);
                }
                fputs(SETCLR_NAMES[i], stdout);
            }
        }
        fputs("\r\n", stdout);
        return SCPI_RES_OK;
    }

    uint8_t value = SCPI_IsCmd(context, "TEST:SET") ? 1 : 0;
    value ^= SETCLR_POL[choice];    // Invert if pol=1

    util_set_pin(SETCLR_PINS[choice], value);
    return SCPI_RES_OK;
}

/**
 * SCPI: Send arbitrary SPI data.
 * Test:SPI DATA
 * 
 * \param context   Active SCPI context
 * \return  Success or failure
 */
scpi_result_t TEST_SPI(scpi_t *context)
{
    int32_t val;
    if (!SCPI_ParamInt(context, &val, true)) {
        return SCPI_RES_ERR;
    }
    uint8_t val_byte = (uint8_t) val;
    printf("Transmitting value %02x\r\n", val_byte);
    spi_write(SPI_MASTER_BASE, val_byte, 0, 0);
    return SCPI_RES_OK;   
}

/**
 * SCPI: Initialize DDS interface
 * Test:INIF
 * 
 * \param context   Active SCPI context
 * \return  Success or failure
 */
scpi_result_t TEST_INIF(scpi_t *context)
{
    (void) context;
    synth_initialize_interface();
    return SCPI_RES_OK;   
}

/**
 * SCPI: Initialize DDS system clock
 * Test:INCK
 * 
 * \param context   Active SCPI context
 * \return  Success or failure
 */
scpi_result_t TEST_INCK(scpi_t *context)
{
    (void) context;
    synth_initialize_clock();
    return SCPI_RES_OK;   
}

/**
 * SCPI: Set DDS frequency
 * Test:FREQ channel, frequency
 * 
 * \param context   Active SCPI context
 * \return  Success or failure
 */
scpi_result_t TEST_FREQ(scpi_t *context)
{
    int32_t ch;
    unsigned ch_uns;
    scpi_number_t freq;

    if (!SCPI_ParamInt(context, &ch, true)) {
        return SCPI_RES_ERR;
    }

    if (!SCPI_ParamNumber(context, &freq, true)) {
        return SCPI_RES_ERR;
    }

    ch_uns = ch;

    printf("Setting frequency %u to %f\r\n", ch_uns, freq.value);
    synth_set_frequency(ch_uns, freq.value);
    return SCPI_RES_OK;
}

/**
 * SCPI: Set DDS phase
 * Test:PHASE channel, phase
 * 
 * \param context   Active SCPI context
 * \return  Success or failure
 */
scpi_result_t TEST_PHASE(scpi_t *context)
{
  int32_t ch;
  unsigned ch_uns;
  scpi_number_t phase;


    if (!SCPI_ParamInt(context, &ch, true)) {
        return SCPI_RES_ERR;
    }

    if (!SCPI_ParamNumber(context, &phase, true)) {
        return SCPI_RES_ERR;
    }

    ch_uns = ch;
    printf("Setting phase %u to %f\r\n", ch_uns, phase.value);
    synth_set_phase(ch_uns, phase.value);
    return SCPI_RES_OK;

}

/**
 * SCPI: Set DDS amplitude
 * Test:AMPlitude channel, amp
 * 
 * \param context   Active SCPI context
 * \return  Success or failure
 */
scpi_result_t TEST_AMPLITUDE(scpi_t *context)
{
  int32_t ch;
  unsigned ch_uns;
  scpi_number_t amplitude;


    if (!SCPI_ParamInt(context, &ch, true)) {
        return SCPI_RES_ERR;
    }

    if (!SCPI_ParamNumber(context, &amplitude, true)) {
        return SCPI_RES_ERR;
    }

    ch_uns = ch;
    printf("Setting amplitude %u to %f\r\n", ch_uns, amplitude.value);
    synth_set_amplitude(ch_uns, amplitude.value);
    return SCPI_RES_OK;

}

/**
 * SCPI: Sample the input.
 * Test:SAMple
 *
 * \param context   Active SCPI context
 * \return  Success or failure
 */
scpi_result_t TEST_SAMPLE(scpi_t *context)
{
    (void) context;
    printf ("%f\r\n", acq_get_one_value ());
    return SCPI_RES_OK;
}
