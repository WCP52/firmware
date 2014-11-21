#include <string.h>
#include "asf.h"
#include "conf_board.h"

#include "synth.h"

/* Toggle the IO_UPDATE pin and trigger the AD9958 to load data into its
 * registers. */
static void io_update (void)
{
	delay_us (1);
	gpio_set_pin_high (GPIO_SYNTH_IOUPDATE);
	delay_us (1);
	gpio_set_pin_low (GPIO_SYNTH_IOUPDATE);
}

/* Wait for a SPI transaction to finish */
static void spi_wait (void)
{
	while (!spi_is_tx_empty (SPI_MASTER_BASE));
}

void synth_initialize_interface (void)
{
    /* Ensure sane pin defaults */
	gpio_set_pin_low (GPIO_SYNTH_PWRDN);
	gpio_set_pin_low (GPIO_SYNTH_nCS);
	gpio_set_pin_low (GPIO_SYNTH_IOUPDATE);

    /* Perform a master reset */
	gpio_set_pin_high (GPIO_SYNTH_MRST);
	delay_us (1);
	gpio_set_pin_low (GPIO_SYNTH_MRST);
	delay_us (1);

    /* Configure standard SPI mode */
	spi_write (SPI_MASTER_BASE, 0x00, 0, 0);
	spi_write (SPI_MASTER_BASE, 0x02, 0, 0);

    spi_wait ();
    io_update ();
}
