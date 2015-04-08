/**
 * @file
 * SCPI LOWlevel:* commands
 */

#ifndef _SCPI_LOWLEVEL_H
#define _SCPI_LOWLEVEL_H 1

#include "scpi/scpi.h"

scpi_result_t LOWLEVEL_PIN_ACTION (scpi_t *context);
scpi_result_t LOWLEVEL_LIST_PINS (scpi_t *context);

#endif // _SCPI_LOWLEVEL_H

