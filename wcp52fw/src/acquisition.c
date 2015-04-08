/**
 * \file
 * Signal acquisition-related functions
 */

// ASF includes
#include <adc.h>
#include <pmc.h>
#include <sysclk.h>

#include <inttypes.h>
#include "conf_board.h"

#include "acquisition.h"

static volatile uint8_t RECEIVED = 0;
static volatile uint32_t ADCVAL = 0;

void ADC_Handler(void)
{
    if (adc_get_status(ADC) & ADC_ISR_DRDY) {
        uint32_t result = adc_get_latest_value(ADC);
        ADCVAL = result;
        RECEIVED = 1;
    }
}

void adc_setup(void)
{
    pmc_enable_periph_clk(ID_ADC);
    adc_init(ADC, sysclk_get_main_hz(), ADC_CLOCK, 8);
    adc_configure_timing(ADC, 0, ADC_SETTLING_TIME_3, 1);
    adc_set_resolution(ADC, ADC_MR_LOWRES_BITS_12);
    adc_enable_channel(ADC, ADC_CHANNEL_3); // TODO: wrong channel
    adc_enable_interrupt(ADC, ADC_IER_DRDY);
    adc_configure_trigger(ADC, ADC_TRIG_SW, 0);
    NVIC_EnableIRQ(ADC_IRQn);
    adc_start(ADC);
}


double acq_get_values (unsigned count)
{
    double total = 0.0;
    size_t n;
    for (n = 0; n < count; ++n) {
        RECEIVED = 0;
        adc_start(ADC);
        while (!RECEIVED);
        total += ADCVAL;
    }
    total /= count;
    return total;
}

