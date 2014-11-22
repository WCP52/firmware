/**
 * \file
 * \brief SCPI Test:* commands
 */

#ifndef _SCPI_TEST_H
#define _SCPI_TEST_H 1

scpi_result_t TEST_LED (scpi_t *context);
scpi_result_t TEST_SET (scpi_t *context);
scpi_result_t TEST_SPI (scpi_t *context);
scpi_result_t TEST_INIF (scpi_t *context);
scpi_result_t TEST_INCK (scpi_t *context);
scpi_result_t TEST_FREQ (scpi_t *context);

#endif /* _SCPI_TEST_H */
