/**
 * \file
 * \brief AD9958 synthesizer interface
 */

#ifndef _WCP52_SYNTH_H
#define _WCP52_SYNTH_H 1

/**
 * Initialize the AD9958 interface.
 * This performs a master reset and sets the interface mode on the AD9958.
 */
void synth_initialize_interface (void);

#endif /* _WCP52_SYNTH_H */
