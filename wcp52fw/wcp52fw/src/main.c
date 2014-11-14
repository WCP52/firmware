/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include <stdio_serial.h>
#include <ioport.h>

int main (void)
{
	// Insert system clock initialization code here (sysclk_init()).

	sysclk_init ();
	board_init ();

	const usart_serial_options_t usart_serial_options = {
		.baudrate   = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY,
	};
	
	stdio_serial_init (CONF_UART, &usart_serial_options);
	
	ioport_init ();
	ioport_set_pin_dir (LED0_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir (PIN_USART0_TXD_IDX, IOPORT_DIR_OUTPUT);

	// Insert application code here, after the board has been initialized.
	for (;;) {
		ioport_toggle_pin_level (LED0_GPIO);
		printf ("TEST\n");
	}
}
