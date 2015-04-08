// Atmel ASF includes
#include <gpio.h>
#include <stdio_usb.h>

#include <inttypes.h>
#include "udi_cdc.h"
#include "usb_protocol_cdc.h"
#include "conf_board.h"

volatile bool G_CDC_ENABLED = false;

void main_sof_action(void) { }

void main_resume_action(void) { }

void main_suspend_action(void) { }

bool callback_cdc_enable(uint8_t port)
{
    (void) port;
    stdio_usb_enable();
    gpio_set_pin_high(GPIO_LED2);
    G_CDC_ENABLED = true;
    return true;
}

void callback_cdc_disable(uint8_t port)
{
    (void) port;
    G_CDC_ENABLED = false;
    gpio_set_pin_low(GPIO_LED2);
    stdio_usb_disable();
}

void callback_cdc_set_coding_ext(uint8_t port, usb_cdc_line_coding_t *cfg)
{
    (void) port;
    (void) cfg;
}

void callback_cdc_set_dtr(uint8_t port, bool enable)
{
    (void) port;
    (void) enable;
}

void callback_cdc_rx_notify(uint8_t port)
{
    (void) port;
}

