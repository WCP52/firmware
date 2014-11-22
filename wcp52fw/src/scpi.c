#include "asf.h"
#include <stdio.h>
#include <inttypes.h>
#include "scpi/scpi.h"
#include "scpi-def.h"

/* These functions are required by the SCPI library to interact with the
 * console. */

size_t SCPI_Write (scpi_t *context, const char * data, size_t len) {
    (void) context;
    return fwrite (data, 1, len, stdout);
}

scpi_result_t SCPI_Flush (scpi_t *context) {
    (void) context;
    return SCPI_RES_OK;
}

int SCPI_Error (scpi_t *context, int_fast16_t err) {
    (void) context;
    fprintf(stderr, "**ERROR: %" PRIdFAST16 ", \"%s\"\r\n", err, SCPI_ErrorTranslate(err));
    return 0;
}

scpi_result_t SCPI_Control(scpi_t * context, scpi_ctrl_name_t ctrl, scpi_reg_val_t val) {
    (void) context;
    if (SCPI_CTRL_SRQ == ctrl) {
        fprintf(stderr, "**SRQ: 0x%X (%d)\r\n", val, val);
    } else {
        fprintf(stderr, "**CTRL %02x: 0x%X (%d)\r\n", ctrl, val, val);
    }
    return SCPI_RES_OK;
}

scpi_result_t SCPI_Test(scpi_t * context) {
    (void) context;
    fprintf(stderr, "**Test\r\n");
    return SCPI_RES_OK;
}

scpi_result_t SCPI_Reset(scpi_t * context) {
    (void) context;

    /* Here, we are going to reset the entire chip. This requires writing to
     * the RSTC (reset controller) control registers. */

    /* Zero out the control register first, so we don't reset prematurely */
    RSTC->RSTC_CR = 0;

    /* Set the mode register */
    RSTC->RSTC_MR = 0
        | RSTC_MR_KEY_PASSWD    /* Chip won't reset without the password */
        ;

    /* Set the control register */
    RSTC->RSTC_CR = 0
        | RSTC_CR_KEY_PASSWD    /* Chip won't reset without the password */
        | RSTC_CR_PROCRST       /* Reset processor */
        ;

    /* This return won't happen, of course :D */
    return SCPI_RES_OK;
}

scpi_result_t SCPI_SystemCommTcpipControlQ(scpi_t * context) {
    (void) context;
    return SCPI_RES_ERR;
}
