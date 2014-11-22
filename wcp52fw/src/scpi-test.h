/**
 * \file
 * \brief SCPI Test:* commands
 */

#ifndef _SCPI_TEST_H
#define _SCPI_TEST_H 1

scpi_result_t TEST_LEDON (scpi_t *context);
scpi_result_t TEST_LEDOFF (scpi_t *context);
scpi_result_t TEST_SETNCS (scpi_t *context);
scpi_result_t TEST_SETIOUP (scpi_t *context);
scpi_result_t TEST_SETPWRDN (scpi_t *context);
scpi_result_t TEST_SETMRST (scpi_t *context);
scpi_result_t TEST_SPI (scpi_t *context);
scpi_result_t TEST_INIF (scpi_t *context);
scpi_result_t TEST_INCK (scpi_t *context);
scpi_result_t TEST_FREQ (scpi_t *context);

#endif /* _SCPI_TEST_H */
