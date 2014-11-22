#include <string.h>
#include "asf.h"
#include "conf_board.h"

#include "synth.h"

/* Synthesizer registers */
uint8_t FR1[FR1_LEN];
uint8_t CFR[CFR_LEN];
uint8_t CFTW0[CFTW_LEN];
uint8_t CFTW1[CFTW_LEN];

/* Toggle the IO_UPDATE pin and trigger the AD9958 to load data into its
 * registers. */
static void io_update (void)
{
	delay_us (1);
	gpio_set_pin_high (GPIO_SYNTH_IOUPDATE);
	delay_us (1);
	gpio_set_pin_low (GPIO_SYNTH_IOUPDATE);
}

/**
 * Wait for a SPI transaction to finish */
static void spi_wait (void)
{
	while (!spi_is_tx_empty (SPI_MASTER_BASE));
}

/**
 *  Send a control register to the AD9958.
 *  @param addr Register address
 *  @param data Register data array
 *  @param data_length  Length of data array
 */
static void send_control_register (uint8_t addr, const uint8_t *data, size_t data_length)
{
    spi_write (SPI_MASTER_BASE, addr, 0, 0);
    for (size_t i = 0; i < data_length; ++i) {
        spi_write (SPI_MASTER_BASE, data[i], 0, 0);
    }
    spi_wait ();
    io_update ();
}

/**
 * Send a channel register to the AD9958.
 * @param addr Register address
 * @param data Register data array
 * @param data_len Length of data array
 * @param channel_num Channel number (0 or 1)
 */
static void send_channel_register (uint8_t addr, const uint8_t *data,
        size_t data_length, unsigned channel_num)
{
    if (channel_num > 1) return;

    /* First, set the proper 'channel enable' bit */
    spi_write (SPI_MASTER_BASE, 0x00, 0, 0);
    spi_write (SPI_MASTER_BASE, channel_num ? 0x42 : 0x82, 0, 0);

    /* Now, send the data */
    spi_write (SPI_MASTER_BASE, addr, 0, 0);
    for (size_t i = 0; i < data_length; ++i) {
        spi_write (SPI_MASTER_BASE, data[i], 0, 0);
    }

    spi_wait ();
    io_update ();
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

void synth_initialize_clock (void)
{
    memset (FR1, 0, sizeof FR1);
    REGSET (FR1, FR1_VCOGAIN, 1); /* VCO gain set for high frequency */
    REGSET (FR1, FR1_PLLRATIO, 20); /* PLL: 25 MHz crystal * 20 = 500 MHz */

    send_control_register (FR1_ADDR, FR1, FR1_LEN);
}

void synth_set_frequency (unsigned channel, double freq)
{
    if (channel > 1) return;

    /* First, convert 'freq' to a frequency tuning word */
    /* From AD9958 datasheet, page 18: Fout = (FTW)(Fsys) / 2^32
     * So FTW = 2^32 * Fout / Fsys
     */
    double ftw = (4294967296. * freq) / SYSCLK_FREQ;
    uint32_t ftw32 = (uint32_t) ftw;

    uint8_t *cftw = channel ? CFTW1 : CFTW0;
    cftw[0] = (ftw32 & 0xff000000uL) >> 24;
    cftw[1] = (ftw32 & 0x00ff0000uL) >> 16;
    cftw[2] = (ftw32 & 0x0000ff00uL) >> 8;
    cftw[3] = (ftw32 & 0x000000ffuL);

    send_channel_register (CFTW_ADDR, cftw, 4, channel);
}
