/**
 * \file
 * \brief AD9958 synthesizer interface
 */

#ifndef _WCP52_SYNTH_H
#define _WCP52_SYNTH_H 1

#define SYSCLK_FREQ 500000000uL

/**
 * Initialize the DDS interface.
 */
void synth_initialize_interface(void);

/**
 * Initialize the DDS system clock.
 */
void synth_initialize_clock(void);

/**
 * Set the DDS frequency on a channel.
 * \param channel   Channel number, either 0 or 1
 * \param freq      Frequency in Hz.
 */
void synth_set_frequency(unsigned channel, double freq);
/**
 * Set the DDS phase on a channel.
 * \param channel   Channel number, either 0 or 1
 * \param phase     phase in degrees.
 */
void synth_set_phase(unsigned channel, double phase);
/**
 * Set the DDS amplitude on a channel.
 * \param channel   Channel number, either 0 or 1
 * \param phase     phase in degrees.
 */
void synth_set_amplitude(unsigned channel, double amplitude);
/**
 * \addtogroup FR1 Function register 1
 * \{ */
#define FR1_LEN 3
#define FR1_ADDR 0x01
#define FR1_VCOGAIN_I 0
#define FR1_VCOGAIN_B 7
#define FR1_CHARGE_PUMP_CTRL_I 0
#define FR1_CHARGE_PUMP_CTRL_B 0
#define FR1_PLLRATIO_I 0
#define FR1_PLLRATIO_B 2
/** \} */


/**
 * \addtogroup CHC Channel control registers
 * \{ */
#define CFR_LEN 3
#define CFR_ADDR 0x03
#define CFR_DAC_FS_I 1
#define CFR_DAC_FS_B 0

#define CFTW_LEN 4
#define CFTW_ADDR 0x04

#define CPOW_LEN 2
#define CPOW_ADDR 0x05

#define CACR_LEN 3
#define CACR_ADDR 0x06
/** \} */

/* Register set macro */
/**
 * Set a value into a register.
 * \param reg   Register variable to set
 * \param field Name of field to set
 * \param value Value to put there
 */
#define REGSET(reg, field, value) ((reg)[field ## _I] |= ((value) << (field ## _B)))

#endif /* _WCP52_SYNTH_H */
