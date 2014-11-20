/**
 * \file
 *
 * \brief Serial Peripheral Interface (SPI) example for SAM.
 *
 * Copyright (c) 2011-2014 Atmel Corporation. All rights reserved.
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

/**
 * \mainpage SPI Example
 *
 * \par Purpose
 *
 * This example uses Serial Peripheral Interface (SPI) of one EK board in
 * slave mode to communicate with another EK board's SPI in master mode.
 *
 * \par Requirements
 *
 * This package can be used with two SAM evaluation kits boards.
 * Please connect the SPI pins from one board to another.
 * \copydoc spi_example_pin_defs
 *
 * \par Descriptions
 *
 * This example shows control of the SPI, and how to configure and
 * transfer data with SPI. By default, example runs in SPI slave mode,
 * waiting SPI slave & UART inputs.
 *
 * The code can be roughly broken down as follows:
 * <ul>
 * <li> 't' will start SPI transfer test.
 * <ol>
 * <li>Configure SPI as master, and set up SPI clock.
 * <li>Send 4-byte CMD_TEST to indicate the start of test.
 * <li>Send several 64-byte blocks, and after transmitting the next block, the
 * content of the last block is returned and checked.
 * <li>Send CMD_STATUS command and wait for the status reports from slave.
 * <li>Send CMD_END command to indicate the end of test.
 * </ol>
 * <li>Setup SPI clock for master.
 * </ul>
 *
 * \par Usage
 *
 * -# Compile the application.
 * -# Connect the UART port of the evaluation board to the computer and open
 * it in a terminal.
 *    - Settings: 115200 bauds, 8 bits, 1 stop bit, no parity, no flow control.
 * -# Download the program into the evaluation board and run it.
 * -# Upon startup, the application will output the following line on the
 *    terminal:
 *    \code
	-- Spi Example  --
	-- xxxxxx-xx
	-- Compiled: xxx xx xxxx xx:xx:xx --
\endcode
 * -# The following traces detail operations on the SPI example, displaying
 *    success or error messages depending on the results of the commands.
 *
 */

#include "asf.h"
#include "stdio_serial.h"
#include "conf_board.h"
#include "conf_clock.h"
#include "conf_spi_example.h"
 
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/* Chip select. */
#define SPI_CHIP_SEL 0
#define SPI_CHIP_PCS spi_get_pcs(SPI_CHIP_SEL)

/* Clock polarity. */
#define SPI_CLK_POLARITY 0

/* Clock phase. */
#define SPI_CLK_PHASE 0

/* Delay before SPCK. */
#define SPI_DLYBS 0x40

/* Delay between consecutive transfers. */
#define SPI_DLYBCT 0x10

/* Slave data mask. */
#define CMD_DATA_MSK 0xFFFF0000

/* Slave data block mask. */
#define DATA_BLOCK_MSK 0x0000FFFF

/* Number of commands logged in status. */
#define NB_STATUS_CMD   20

/* Number of SPI clock configurations. */
#define NUM_SPCK_CONFIGURATIONS 4

/* SPI Communicate buffer size. */
#define COMM_BUFFER_SIZE   64

/* UART baudrate. */
#define UART_BAUDRATE      115200

/* Data block number. */
#define MAX_DATA_BLOCK_NUMBER  4

/* Max retry times. */
#define MAX_RETRY    4

#define STRING_EOL    "\r"
#define STRING_HEADER "--Spi Example --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

/* Status block. */
struct status_block_t {
	/** Number of data blocks. */
	uint32_t ul_total_block_number;
	/** Number of SPI commands (including data blocks). */
	uint32_t ul_total_command_number;
	/** Command list. */
	uint32_t ul_cmd_list[NB_STATUS_CMD];
};

/* SPI clock setting (Hz). */
static uint32_t gs_ul_spi_clock = 500000;

/* SPI Status. */
static struct status_block_t gs_spi_status;

static uint32_t gs_ul_test_block_number;

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
			(sysclk_get_cpu_hz() / gs_ul_spi_clock));
	spi_set_transfer_delay(SPI_MASTER_BASE, SPI_CHIP_SEL, SPI_DLYBS,
			SPI_DLYBCT);
	spi_enable(SPI_MASTER_BASE);
}
/********************************************************************
*********************************************************************
*********DEFINITIONS TO FOR COMMUNICATION WITH SYTHESIZER BOARD******
*********************************************************************
*********************************************************************/

//instruction byte to write to Channel Select Register
#define SCR_WRITE 0x80

//command to turn LSB mode on
#define LSB_MODE_ON 0x01

//instruction byte to write to Function Register 1
#define FR1_WRITE 0x81

//command to set 500 Mhz clock
#define SET_SYSCLK 0xD

//enables configuration of ch0/ch1
#define CH0_ENABLE 0x41
#define CH1_ENABLE 0x81 

//instruction byte to write to channel frequency tuning word register
#define CFTW_WRITE 0x84

static void io_update(void)
{
	printf("\nExpecting an IO update here.  Gotta figure out which PIO port is doing it. Writing the function is easy from there\n");
	/*
	set io_update pin to 1, wait for a microsecond and set io_update pin to 0
	*/
}


/*synth_init initializes the synthesizer before use.
	inputs - none
	outputes - none
	This function MUST be called before using the synthesizer chip
*/
static void synth_init(void)
{
	
	//set synthesizer to LSB mode
	spi_write(SPI_MASTER_BASE, SCR_WRITE, 0, 0);
	spi_write(SPI_MASTER_BASE, LSB_MODE_ON, 0, 0);
	io_update();
	
	//set sysclk speed for synthesizer to 500 MHz
	spi_write(SPI_MASTER_BASE, FR1_WRITE, 0, 0);
	spi_write(SPI_MASTER_BASE, 0x00, 0, 0);
	spi_write(SPI_MASTER_BASE, SET_SYSCLK, 0, 0);
	io_update();
	
}

/* sets a channel to output a given frequency
	inputs - cmd_arg[4]: determines the channel to set
		     cmd_value : determines the frequency in MHz
	outputs - none
*/
static void set(char cmd_arg[4], int cmd_value)
{
	//First enable the channel that we want to set
	if (!strcmp(cmd_arg, "ch0"))
	{
		spi_write(SPI_MASTER_BASE, SCR_WRITE, 0, 0);
		spi_write(SPI_MASTER_BASE, CH0_ENABLE, 0, 0);
		
	}
	else if (!strcmp(cmd_arg, "ch1"))
	{
		spi_write(SPI_MASTER_BASE, SCR_WRITE, 0, 0);
		spi_write(SPI_MASTER_BASE, CH1_ENABLE, 0, 0);
	}
	else
	{
		printf("\n%s is not a valid argument\n", cmd_arg);
		return;
	}
	
	//write the necessary FTW
	uint8_t i;
	uint32_t FTW = (cmd_value*4294967296)/500;
	spi_write(SPI_MASTER_BASE, CFTW_WRITE, 0, 0);
	for (i = 0; i < 32; i++)
	{
		spi_write(SPI_MASTER_BASE, FTW & (1 << i), 0, 0);
	}
	io_update();
}

static void sweep(char cmd_arg[4])
{
	//First, enable the channel that will do the sweep
	if (!strcmp(cmd_arg, "ch0"))
	{
		spi_write(SPI_MASTER_BASE, SCR_WRITE, 0, 0);
		spi_write(SPI_MASTER_BASE, CH0_ENABLE, 0, 0);
	}
	else if (!strcmp(cmd_arg, "ch1"))
	{
		spi_write(SPI_MASTER_BASE, SCR_WRITE, 0, 0);
		spi_write(SPI_MASTER_BASE, CH1_ENABLE, 0, 0);
	}
	else
	{
		printf("\n%s is not a valid argument\n", cmd_arg);
		return;
	}
	uint8_t i, j;
	for (i = 1; i <= 150; i++)
	{
		uint32_t FTW = (i*4294967296)/500;
		spi_write(SPI_MASTER_BASE, CFTW_WRITE, 0, 0);
		for (j = 0; j < 32; j++)
		{
			spi_write(SPI_MASTER_BASE, FTW & (1 << j), 0, 0);
		}	
		printf("\nSending FTW = %d to synth\n", FTW);
		io_update();
	}
}

/* parse_cmd allows us to parse commands from the terminal.
Currently, there will only be 2 commands to parse.  set and sweep.
	Sytnax:
		set ch freq,
			set - sets a channel to a given frequency
			ch  - either ch0 or ch1 for the channel we want to set
			freq - the desired frequency in Mhz
		sweep ch
			sweep - makes a channel sweep from 0 0 Hz to 150 Mhz
			ch    - channel to do the sweep
*/ 
static void parse_cmd(void)
{
	char cmd_buf[5];
	char cmd_arg[4];
	int cmd_value;
	
	scanf("%s %s %d", &cmd_buf, &cmd_arg, &cmd_value);
	
	if (!strcmp(cmd_buf, "set"))
	{
		set(cmd_arg, cmd_value);
	}
	else if (!strcmp(cmd_buf, "sweep"))
	{
		sweep(cmd_arg);
	}
	else
		printf("\n%d is not a valid command\n", cmd_buf);
	return;
}

/**
 * \brief Application entry point for SPI example.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	uint8_t uc_key;

	/* Initialize the SAM system. */
	sysclk_init();
	board_init();

	spi_master_initialize();
	synth_init();
	
	while(1)
	{
		parse_cmd();	
	}
	return 0;
}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
