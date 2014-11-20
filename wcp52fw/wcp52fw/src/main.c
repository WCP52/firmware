/**
 * \file
 *
 * \brief ADC temperature sensor example for SAM.
 *
 * Copyright (c) 2011 - 2014 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "asf.h"
#include "conf_board.h"

/**
 * \brief Systick handler, start new conversion.
 */
void SysTick_Handler(void)
{
}

/**
 * \brief Configure UART console.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 * \brief Get a line from serial
 * \param buffer	Buffer which will receive line
 * \param buflen	Length of buffer
 * \return 0 on success, 1 on error
 */
int get_line_from_serial (char *buffer, size_t buflen);
int get_line_from_serial (char *buffer, size_t buflen)
{
	/* Implement our own getline so we can easily complain if the length goes
	 * over. */
	size_t i;
	for (i = 0; i < buflen - 1; ++i) {
		int char_from_serial = getchar ();
		if (char_from_serial < 0 || char_from_serial == EOF) {
			puts ("Serial error!\r");
			return 1;
		}
		if (char_from_serial == '\r' || char_from_serial == '\n') {
			/* End of line */
			buffer[i] = 0;
			break;
		}
		buffer[i] = (char) char_from_serial;
	}
	
	if (i == buflen - 1) {
		/* Reached the end without getting a newline. */
		puts ("Buffer overflow!\r");
		return 1;
	} else {
		return 0;
	}
}

/**
 * \brief Read a line from stdin and interpret it.
 */
void cmd_process (void);
void cmd_process (void)
{
	char serial_buffer[256];
	
	if (get_line_from_serial (serial_buffer, sizeof serial_buffer)) {
		return;
	}
	
	/* First character is the command */
	char cmd = serial_buffer [0];
	
	/* Rest is the argument */
	char *arg = serial_buffer + 1;
	
	/* Advance the argument until no more spaces exist */
	while (*arg) {
		if (!isspace ((int) arg[0])) break;
		++arg;
	}
	
	/* Branch! */
	switch (cmd) {
		case 'e':
		case 'E':
			/* Echo */
			puts (arg);
			puts ("\r");
	}
}

/**
 * \brief adc_temp_sensor Application entry point.
 *
 * Initialize adc to 12-bit, enable channel 15,turn on
 * temp sensor, pdc channel interrupt for temp sensor
 * and start conversion.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	/* Initialize the SAM system. */
	sysclk_init();
	board_init();

	configure_console();

	/* 10 ms timer */
	if (SysTick_Config(sysclk_get_cpu_hz() / 100)) {
		for (;;) {
			puts("-F- Systick configuration error\r");
		}
	}

	for (;;) {
		cmd_process ();
	}
}
