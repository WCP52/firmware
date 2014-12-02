/**
 * \file
 * SCPI Test:* commands
 */

#ifndef _SCPI_TEST_H
#define _SCPI_TEST_H 1

#include "scpi/scpi.h"

scpi_result_t TEST_SETCLR (scpi_t *context);
scpi_result_t TEST_SPI (scpi_t *context);
scpi_result_t TEST_INIF (scpi_t *context);
scpi_result_t TEST_INCK (scpi_t *context);
scpi_result_t TEST_FREQ (scpi_t *context);
scpi_result_t TEST_PHASE (scpi_t *context);
scpi_result_t TEST_AMPLITUDE (scpi_t *context);
scpi_result_t TEST_SAMPLE (scpi_t *context);

#endif // _SCPI_TEST_H
