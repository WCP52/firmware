#include <inttypes.h>
#include <asf.h>
#include "udi_cdc.h"
#include "usb_protocol_cdc.h"

void main_sof_action (void)
{}

void main_resume_action (void)
{}

void main_suspend_action (void)
{}

static bool authorize_cdc_transfert = false;

bool callback_cdc_enable (uint8_t port)
{
    (void) port;
    authorize_cdc_transfert = true;
    return true;
}

void callback_cdc_disable (uint8_t port)
{
    (void) port;
    authorize_cdc_transfert = false;
}

void callback_cdc_set_coding_ext(uint8_t port, usb_cdc_line_coding_t *cfg)
{
    // This function is called when the CDC interface has been configured.
    // It doesn't *need* to do anything, but can be used if you want to
    // know the setup
    (void) port;
    (void) cfg;
}

void callback_cdc_set_dtr(uint8_t port, bool b_enable)
{
    (void) port;
    (void) b_enable;
}

void callback_cdc_rx_notify(uint8_t port)
{
    (void) port;
}

void task (void)
{
    if (authorize_cdc_transfert) {
        udi_cdc_putc('A');
        udi_cdc_getc();
    }
}
