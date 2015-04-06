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
#include <udc.h>

#include "synth.h"
#include "util.h"

static void pins_init(void);

/**
 * Initialize the board
 */
// Cannot declare static, as board.h declares this extern
void board_init(void)
{
    // Disable watchdog timer
    WDT->WDT_MR = WDT_MR_WDDIS;

    ioport_init();
    pins_init();
    pmc_enable_periph_clk(ID_PIOB);
}

/**
 * Initialize the UART stdio console.
 */
static void console_init(void)
{
    stdio_usb_init();
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
 * Configure all device pins
 */
static void pins_init(void)
{
    /* Configure pins */
#define XPINGROUP(desc)
#define XPIN(name, pin, setting, comment) gpio_configure_pin(PIO_ ## pin ## _IDX, (setting));
PIN_LIST
#undef XPIN
#undef XPINGROUP
}

extern bool cdc_enabled;

/**
 * Main function.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
    // Initialize all used peripherals.
    sysclk_init();
    irq_initialize_vectors();
    cpu_irq_enable();
    board_init();
    spi_init();
    adc_setup();
    //udc_start();
    console_init();
    gpio_set_pin_high(GPIO_LED1);
    
    SCPI_Init(&G_SCPI_CONTEXT);

    //puts("**Initialization successful\r");

    const size_t SMBUFFER_SIZE = 10;
    char smbuffer[10];
    for (;;) {
        /* Get into smbuffer until either full, or a \r or \n */
        size_t i;
        for (i = 0; i < SMBUFFER_SIZE - 1; ++i) {
            char ch = 0;
            if (cdc_enabled) {
                ch = udi_cdc_getc();
            } else {
                --i;
                continue;
            }
            if (!ch) continue;
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

