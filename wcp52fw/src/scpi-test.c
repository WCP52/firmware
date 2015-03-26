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
    int32_t num_samples;
    if (!SCPI_ParamInt(context, &num_samples, true)) {
        return SCPI_RES_ERR;
    }

    printf ("%f\r\n", acq_get_values (num_samples));
    return SCPI_RES_OK;
}

/**
 * SCPI: Set active input channel.
 * TEST:CHannel
 *
 * \param context   Active SCPI context
 * \return  Success or failure
 */
scpi_result_t TEST_CHANNEL(scpi_t *context)
{
  int32_t ch;


    if (!SCPI_ParamInt(context, &ch, true)) {
        return SCPI_RES_ERR;
    }


    util_set_pin (GPIO_CHANSEL, ch);

    return SCPI_RES_OK;
}
