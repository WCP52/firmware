/**
 * \file
 *
 * \brief WCP52 gain/phase main loop
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "asf.h"
#include "conf_board.h"

/*TODO: Move to synth_regs.c*/
#include "synth_regs.h"
#include "synth.h"

uint8_t FR1[3] = {0};
uint8_t CFR[3] = {0};
uint8_t CFTW0[4] = {0};	


struct spi_device spi_dev;

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
 * \brief Set LED on or off
 */
void do_cmd_led (char *arg, int on);
void do_cmd_led (char *arg, int on)
{
	int led = atoi (arg);
	if (led == 0) {
		if (on) {
			gpio_set_pin_high (LED0_GPIO);
		} else {
			gpio_set_pin_low (LED0_GPIO);
		}
	} else if (led == 1) {
		if (on) {
			gpio_set_pin_high (LED1_GPIO);
			} else {
			gpio_set_pin_low (LED1_GPIO);
		}
	}
}

/**
 * \brief Send data
 */
void do_cmd_spi (char *arg);
void do_cmd_spi (char *arg)
{
	long int arg_as_num = strtol (arg, NULL, 16);
	if (arg_as_num >= 0 && arg_as_num <= 255) {
		spi_write (SPI_MASTER_BASE, (uint8_t) arg_as_num, 0, 0);
	}
}

void sendControlRegister(uint8_t addr, const uint8_t *data, size_t data_length);
void sendControlRegister(uint8_t addr, const uint8_t *data, size_t data_length)
{
	//TODO: syncio 
	gpio_set_pin_low(GPIO_SYNTH_nCS);
	spi_write(SPI_MASTER_BASE, addr, 0, 0);
	for (size_t i = 0; i < data_length; i++)
	{
		spi_write(SPI_MASTER_BASE, data[i], 0, 0);
	}
	while (!spi_is_tx_empty (SPI_MASTER_BASE));
	delay_us (1);
	gpio_set_pin_high (GPIO_SYNTH_IOUPDATE);
	delay_us (1);
	gpio_set_pin_low (GPIO_SYNTH_IOUPDATE);
}

void sendChannelRegister(uint8_t addr, const uint8_t *data, size_t data_length, uint8_t channelNumber);
void sendChannelRegister(uint8_t addr, const uint8_t *data, size_t data_length, uint8_t channelNumber)
{
	if (channelNumber > 1)
		return;
	gpio_set_pin_low(GPIO_SYNTH_nCS);
	spi_write(SPI_MASTER_BASE, 0, 0, 0);
	if (channelNumber == 0)
		spi_write(SPI_MASTER_BASE, 0x82, 0, 0);
	else 
		spi_write(SPI_MASTER_BASE, 0X42, 0, 0); 
	spi_write(SPI_MASTER_BASE, addr, 0, 0);
	for (size_t i = 0; i < data_length; i++)
	{
		spi_write(SPI_MASTER_BASE, data[i], 0, 0);
	}
	
	while (!spi_is_tx_empty (SPI_MASTER_BASE));
	delay_us (1);
	gpio_set_pin_high (GPIO_SYNTH_IOUPDATE);
	delay_us (1);
	gpio_set_pin_low (GPIO_SYNTH_IOUPDATE);
		
}

void setCh1Freq(void);
void setCh1Freq(void)
{
	sendChannelRegister(CFTW0_ADDR, (uint8_t *) "\0\0\0\x01", 4, 1);
}

void synthClockInit(void);
void synthClockInit(void)
{
	memset (FR1, 0, sizeof FR1);
	FR1[FR1_VCOGAIN_I] |= (1 << FR1_VCOGAIN_B);
	FR1[FR1_PLLRATIO_I] |= (20 << FR1_PLLRATIO_B);
	sendControlRegister(FR1_ADDR, FR1, sizeof(FR1));
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
			break;
		
		case 'L':
			/* Led on */
			do_cmd_led (arg, 1);
			break;
		case 'l':
			/* Led off */
			do_cmd_led (arg, 0);
			break;
			
		case 's':
			/* SPI transmit */
			do_cmd_spi (arg);
			break;
		
		case '1':
			/* Synthesizer init */
            synth_initialize_interface ();
			break;

		case '2':
			synthClockInit();
			break;

		case '3':
			setCh1Freq();
			break;
	}
}

/**
 * \brief Initialize SPI as master.
 */
static void spi_master_initialize(void)
{
	puts("-I- Initialize SPI as master\r");

	/* Configure an SPI peripheral. */
	spi_enable_clock(SPI_MASTER_BASE);
	spi_disable(SPI_MASTER_BASE);
	spi_reset(SPI_MASTER_BASE);
	spi_set_lastxfer(SPI_MASTER_BASE);
	spi_set_master_mode(SPI_MASTER_BASE);
	spi_disable_mode_fault_detect(SPI_MASTER_BASE);
	spi_set_peripheral_chip_select_value(SPI_MASTER_BASE, SPI_CHIP_PCS);
	spi_set_clock_polarity(SPI_MASTER_BASE, SPI_CHIP_SEL, SPI_CLK_POLARITY);
	spi_set_clock_phase(SPI_MASTER_BASE, SPI_CHIP_SEL, SPI_CLK_PHASE);
	spi_set_bits_per_transfer(SPI_MASTER_BASE, SPI_CHIP_SEL,
			SPI_CSR_BITS_8_BIT);
	spi_set_baudrate_div(SPI_MASTER_BASE, SPI_CHIP_SEL,
			(sysclk_get_cpu_hz() / 100000uL));
	spi_set_transfer_delay(SPI_MASTER_BASE, SPI_CHIP_SEL, SPI_DLYBS,
			SPI_DLYBCT);
	spi_enable(SPI_MASTER_BASE);
}



/* GPIO pins for synth control */
void pins_init (void);
void pins_init (void)
{
	gpio_configure_pin (GPIO_SYNTH_nCS, GPIO_SYNTH_nCS_F);
	gpio_configure_pin (GPIO_SYNTH_IOUPDATE, GPIO_SYNTH_IOUPDATE_F);
	gpio_configure_pin (GPIO_SYNTH_PWRDN, GPIO_SYNTH_PWRDN_F);
	gpio_configure_pin (GPIO_SYNTH_MRST, GPIO_SYNTH_MRST_F);
}

/**
 * \brief main Application entry point.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	/* Initialize the SAM system. */
	sysclk_init();
	board_init();
	pins_init ();

	configure_console();
	
	spi_master_initialize ();
	
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
