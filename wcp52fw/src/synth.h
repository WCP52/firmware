/**
 * \file
 * \brief AD9958 synthesizer interface
 */

#ifndef _WCP52_SYNTH_H
#define _WCP52_SYNTH_H 1

#define SYSCLK_FREQ 500000000uL

/**
 * Initialize the AD9958 interface.
 * This performs a master reset and sets the interface mode on the AD9958.
 */
void synth_initialize_interface (void);

/**
 * Initialize the AD9958 clock.
 */
void synth_initialize_clock (void);

/**
 * Set the frequency on a channel.
 * @param channel Channel number, either 0 or 1.
 * @param freq Frequency in Hz.
 */
void synth_set_frequency (unsigned channel, double freq);


/* Synthesizer registers */
#define FR1_LEN 3
#define FR1_ADDR 0x01
#define FR1_VCOGAIN_I 0
#define FR1_VCOGAIN_B 7
#define FR1_CHARGE_PUMP_CTRL_I 0
#define FR1_CHARGE_PUMP_CTRL_B 0
#define FR1_PLLRATIO_I 0
#define FR1_PLLRATIO_B 2

#define CFR_LEN 3
#define CFR_ADDR 0x03

//DAC Full scale current control should default 11
#define CFR_DAC_FS_I 1
#define CFR_DAC_FS_B 0

#define CFTW_LEN 4
#define CFTW_ADDR 0x04

/* Register set macro */
#define REGSET(reg, field, value) ((reg)[field ## _I] |= ((value) << (field ## _B)))

#endif /* _WCP52_SYNTH_H */
