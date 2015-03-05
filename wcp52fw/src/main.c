/**
 * \file
 * Main loop and initializers.
 */

#include "scpi/scpi.h"
#include "scpi-def.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "asf.h"
#include "serial.h"
#include "acquisition.h"
#include "conf_board.h"

#include "synth.h"

/**
 * Initialize the UART stdio console.
 */
static void console_init(void)
{
    const usart_serial_options_t uart_serial_options = {
        .baudrate = CONF_UART_BAUDRATE,
        .paritytype = CONF_UART_PARITY,
        .charlength = US_MR_CHRL_8_BIT,
        .stopbits = false
    };

    // Configure console UART.
    sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
    stdio_serial_init(CONF_UART, &uart_serial_options);
}


/**
 * Initialize the SPI controller.
 */
static void spi_init(void)
{
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

/**
 * Initialize the GPIO pins that are not handled by board_init().
 */
static void pins_init(void)
{
    gpio_configure_pin(GPIO_SYNTH_nCS, GPIO_SYNTH_nCS_F);
    gpio_configure_pin(GPIO_SYNTH_IOUPDATE, GPIO_SYNTH_IOUPDATE_F);
    gpio_configure_pin(GPIO_SYNTH_PWRDN, GPIO_SYNTH_PWRDN_F);
    gpio_configure_pin(GPIO_SYNTH_MRST, GPIO_SYNTH_MRST_F);
    gpio_configure_pin(GPIO_SYNTH_SYNCIO, GPIO_SYNTH_SYNCIO_F);
    gpio_configure_pin(GPIO_FRONT_CHAN, GPIO_FRONT_CHAN_F);
}

/**
 * Main function.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
    // Initialize all used peripherals.
    sysclk_init();
    board_init();
    pins_init();
    console_init();
    spi_init();
    adc_setup();
    
    SCPI_Init(&G_SCPI_CONTEXT);

    puts("**Initialization successful\r");

    const size_t SMBUFFER_SIZE = 10;
    char smbuffer[10];
    for (;;) {
        /* Get into smbuffer until either full, or a \r or \n */
        size_t i;
        for (i = 0; i < SMBUFFER_SIZE - 1; ++i) {
            int ch = getchar();
            if (ch == '\r' || ch == '\n') {
                smbuffer[i] = ch;
                ++i;
                break;
            }
            else if (ch > 0 && ch <= 255) {
                smbuffer[i] = ch;
            }
        }
        /* Terminate! */
        smbuffer[i] = 0;
        SCPI_Input(&G_SCPI_CONTEXT, smbuffer, strlen (smbuffer));
    }

    return 0;
}
