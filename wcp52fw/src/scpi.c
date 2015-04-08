// Atmel ASF includes
#include <pmc.h>

#include <stdio.h>
#include <inttypes.h>
#include "scpi/scpi.h"
#include "scpi-def.h"

/* These functions are required by the SCPI library to interact with the
 * console.
 */

/**
 * Write back to the SCPI console.
 * \param context   Active SCPI context
 * \param data      Data to write
 * \param len       Length of data
 * \return  Number of bytes written
 */
size_t SCPI_Write(scpi_t *context, const char * data, size_t len) {
    (void) context;
    return fwrite (data, 1, len, stdout);
}

/**
 * Flush the SCPI console. May be a no-op if not possible or sensible.
 * \param context   Active SCPI context
 * \return  Success or failure; returns success on no-op.
 */
scpi_result_t SCPI_Flush (scpi_t *context) {
    (void) context;
    return SCPI_RES_OK;
}

/**
 * Report a SCPI error. Not all SCPI interfaces report this; some require
 * polling. This may be a no-op in that case.
 * \param context   Active SCPI context
 * \param err       SCPI error number
 * \return  zero
 */
int SCPI_Error (scpi_t *context, int_fast16_t err) {
    (void) context;
    fprintf(stderr, "**ERROR: %" PRIdFAST16 ", \"%s\"\r\n", err, SCPI_ErrorTranslate(err));
    return 0;
}

/**
 * Handle the SCPI control commands.
 * \param context   Active SCPI context
 * \param ctrl      The control command given. See scpi_ctrl_name_t in
 *                  scpi/types.h for a list.
 * \param val       The value passed with the command.
 * \return  Success or failure
 */
scpi_result_t SCPI_Control(scpi_t * context, scpi_ctrl_name_t ctrl, scpi_reg_val_t val) {
    (void) context;
    if (SCPI_CTRL_SRQ == ctrl) {
        fprintf(stderr, "**SRQ: 0x%X (%d)\r\n", val, val);
    } else {
        fprintf(stderr, "**CTRL %02x: 0x%X (%d)\r\n", ctrl, val, val);
    }
    return SCPI_RES_OK;
}

/**
 * Handle the SCPI *TST? command.
 * \param context   Active SCPI context
 * \return  Success or failure
 */
scpi_result_t SCPI_Test(scpi_t * context) {
    (void) context;
    fprintf(stderr, "**Test\r\n");
    return SCPI_RES_OK;
}

/**
 * Handle the SCPI *RST command.
 * \param context   Active SCPI context
 * \return  Success or failure
 */
scpi_result_t SCPI_Reset(scpi_t * context) {
    (void) context;

    // Here, we are going to reset the entire chip. This requires writing to
    // the RSTC (reset controller) control registers.

    // Zero out the control register first, so we don't reset prematurely
    RSTC->RSTC_CR = 0;

    // Set the mode register
    RSTC->RSTC_MR = 0
        | RSTC_MR_KEY_PASSWD    // Chip won't reset without the password
        ;

    // Set the control register
    RSTC->RSTC_CR = 0
        | RSTC_CR_KEY_PASSWD    // Chip won't reset without the password
        | RSTC_CR_PROCRST       // Reset processor
        ;

    // This return won't happen, of course :D
    return SCPI_RES_OK;
}

