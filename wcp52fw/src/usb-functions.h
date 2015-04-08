#ifndef _USB_FUNCTIONS_H
#define _USB_FUNCTIONS_H 1

#include <inttypes.h>

extern volatile bool G_CDC_ENABLED;

/**
 * Start of Frame callback.
 * This is called at the beginning of a USB frame.
 */
void main_sof_action (void);

/**
 * USB resume callback.
 */
void main_resume_action (void);

/**
 * USB suspend callback.
 */
void main_suspend_action (void);

/**
 * CDC protocol enable function.
 * @param port - port number, for multi-port implementations
 * @return true on success
 */
bool callback_cdc_enable (uint8_t port);

/**
 * CDC protocol disable function.
 * @param port - port number, for multi-port implementations
 */
void callback_cdc_disable (uint8_t port);

/**
 * CDC configuration callback.
 * This is called to report the CDC configuration being used. It is not
 * necesary for this function to do anything.
 * @param port - port number, for multi-port implementations
 * @param cfg - CDC configuration
 */
void callback_cdc_set_coding_ext(uint8_t port, usb_cdc_line_coding_t *cfg);

/**
 * CDC DTR-set function
 * Called to set the serial DTR line for out-of-band flow control.
 * @param port - port number, for multi-port implementations
 * @param enable - true if DTR is to be activated
 */
void callback_cdc_set_dtr(uint8_t port, bool enable);

/**
 * CDC data receive notification
 * @param port - port number, for multi-port implementations
 */
void callback_cdc_rx_notify(uint8_t port);

#endif // _USB_FUNCTIONS
