/**
 * \file
 * DDS synthesizer control functions
 */

#include "asf.h"
#include "conf_board.h"
#include <string.h>

#include "synth.h"

// Synthesizer registers
uint8_t G_FR1[FR1_LEN];
uint8_t G_CFR[CFR_LEN];
uint8_t G_CFTW0[CFTW_LEN];
uint8_t G_CFTW1[CFTW_LEN];

uint8_t G_CPOW0[CPOW_LEN];
uint8_t G_CPOW1[CPOW_LEN];


uint8_t G_CACR0[CACR_LEN];
uint8_t G_CACR1[CACR_LEN];


/**
 * Reset the DDS IO system. This aborts a current IO cycle and prepares for
 * the next one.
 */
static void syncio(void)
{
    gpio_set_pin_high(GPIO_SYNTH_SYNCIO);
    gpio_set_pin_low(GPIO_SYNTH_SYNCIO);
}

/**
 * Commit loaded data into the DDS registers.
 */
static void io_update(void)
{
	gpio_set_pin_high(GPIO_SYNTH_IOUPDATE);
	gpio_set_pin_low(GPIO_SYNTH_IOUPDATE);
}

/**
 * Wait for a SPI transaction to finish.
 */
static void spi_wait(void)
{
	while (!spi_is_tx_empty(SPI_MASTER_BASE));
}

/**
 *  Send a control register to the DDS device.
 *  \param addr     Register address
 *  \param data     Register data array
 *  \param data_length  Length of data array
 */
static void send_control_register(
        uint8_t addr, const uint8_t *data, size_t data_length)
{
    gpio_set_pin_low(GPIO_SYNTH_nCS);
    syncio();
    spi_write(SPI_MASTER_BASE, addr, 0, 0);
    for (size_t i = 0; i < data_length; ++i) {
        spi_write(SPI_MASTER_BASE, data[i], 0, 0);
    }
    spi_wait();
    gpio_set_pin_high(GPIO_SYNTH_nCS);
    io_update();
}

/**
 * Send a channel register to the DDS device.
 * @param addr  Register address
 * @param data  Register data array
 * @param data_len      Length of data array
 * @param channel_num   Channel number (0 or 1)
 */
static void send_channel_register(
        uint8_t addr, const uint8_t *data,
        size_t data_length, unsigned channel_num)
{
    if (channel_num > 1) return;

    gpio_set_pin_low(GPIO_SYNTH_nCS);
    syncio();

    // First, set the proper 'channel enable' bit
    spi_write(SPI_MASTER_BASE, 0x00, 0, 0);
    spi_write(SPI_MASTER_BASE, channel_num ? 0x42 : 0x82, 0, 0);

    // Now, send the data
    spi_write(SPI_MASTER_BASE, addr, 0, 0);
    for (size_t i = 0; i < data_length; ++i) {
        spi_write(SPI_MASTER_BASE, data[i], 0, 0);
    }

    spi_wait();
    gpio_set_pin_high(GPIO_SYNTH_nCS);
    io_update();
}

/**
 * Initialize the DDS interface.
 */
void synth_initialize_interface(void)
{
    // Ensure sane pin defaults
	gpio_set_pin_low(GPIO_SYNTH_PWRDN);
	gpio_set_pin_low(GPIO_SYNTH_nCS);
	gpio_set_pin_low(GPIO_SYNTH_IOUPDATE);

    // Make sure to delay after PWRDN goes low.
    // No idea how long! I can't find it in the datasheet...
    delay_ms(50);

    // Perform a master reset
	gpio_set_pin_high(GPIO_SYNTH_MRST);
	gpio_set_pin_low(GPIO_SYNTH_MRST);

    // Configure standard SPI mode
    syncio();
	spi_write(SPI_MASTER_BASE, 0x00, 0, 0);
	spi_write(SPI_MASTER_BASE, 0x02, 0, 0);

    spi_wait();
    gpio_set_pin_high(GPIO_SYNTH_nCS);
    io_update();
}

/**
 * Initialize the DDS system clock.
 */
void synth_initialize_clock(void)
{
    memset(G_FR1, 0, FR1_LEN);
    REGSET(G_FR1, FR1_VCOGAIN, 1); /* VCO gain set for high frequency */
    REGSET(G_FR1, FR1_PLLRATIO, 20); /* PLL: 25 MHz crystal * 20 = 500 MHz */

    send_control_register(FR1_ADDR, G_FR1, FR1_LEN);
}

/**
 * Set the DDS frequency on a channel.
 * \param channel   Channel number, either 0 or 1
 * \param freq      Frequency in Hz.
 */
void synth_set_frequency(unsigned channel, double freq)
{
    if (channel > 1) return;

    // First, convert 'freq' to a frequency tuning word.
    // From AD9958 datasheet, page 18: Fout = (FTW)(Fsys) / 2^32
    // So FTW = 2^32 * Fout / Fsys

    double ftw = (4294967296. * freq) / SYSCLK_FREQ;
    uint32_t ftw32 = (uint32_t) ftw;

    // Prepare the FTW as an array of four bytes
    uint8_t *cftw = channel ? G_CFTW1 : G_CFTW0;
    cftw[0] = (ftw32 & 0xff000000uL) >> 24;
    cftw[1] = (ftw32 & 0x00ff0000uL) >> 16;
    cftw[2] = (ftw32 & 0x0000ff00uL) >> 8;
    cftw[3] = (ftw32 & 0x000000ffuL);

    // Transmit the bytes
    send_channel_register(CFTW_ADDR, cftw, CFTW_LEN, channel);
}


/**
 * Set the DDS phase on a channel.
 * \param channel   Channel number, either 0 or 1
 * \param freq      Phase in degrees.
 */
void synth_set_phase(unsigned channel, double phase)
{

  if (channel > 1) return;
  // convert "phase" to phase offset word
  // equation according to datasheet page 18 
  double pow =  (16384. * phase) / 360 ; 
  uint16_t pow14 = (uint16_t)pow & 0x03FFF;

   //prepare pow as array of 2 bytes 
    uint8_t *cpow = channel ? G_CPOW1 : G_CPOW0;
    cpow[0] = (pow14 & 0x0000ff00uL) >> 8;
    cpow[1] = (pow14 & 0x000000ffuL);
   
    send_channel_register(CPOW_ADDR, cpow, CPOW_LEN, channel);

}

/**
 * Set the DDS amplitude on a channel.
 * \param channel   Channel number, either 0 or 1
 * \param freq      Amplitude
 */
void synth_set_amplitude(unsigned channel, double amplitude)
{
     if (channel > 1) return;
     double acr =  1023 * amplitude ; 
     // 
     uint16_t acr16 = (uint16_t)acr & 0x03FF;
     //setting the amplitude enable bit high
     acr16 |= 0x1000;

    uint8_t *cacr = channel ? G_CACR1 : G_CACR0;
    cacr[0] = 0; // Ramp rate
    cacr[1] = (acr16 & 0x0000ff00uL) >> 8;
    cacr[2] = (acr16 & 0x000000ffuL);

    send_channel_register(CACR_ADDR, cacr, CACR_LEN, channel);
}
