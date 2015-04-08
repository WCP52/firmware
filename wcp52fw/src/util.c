/**
 * \file
 * Miscellaneous utilities
 */

#include "util.h"
#include "conf_board.h"

void for_each_pin (bool (*pinfxn) (const struct pin_info *pin),
        bool (*grpfxn) (const char *text))
{
#define XPINGROUP(_grp) \
    if (grpfxn && !(grpfxn)(_grp)) return;

#define XPIN(_pin, _idx, _flags, _descr) \
    struct pin_info infostruct_ ## _pin = { \
        .pin_name_str = #_pin, \
        .index_str = #_idx, \
        .flags_str = #_flags, \
        .description = _descr, \
        .index = PIO_ ## _idx ## _IDX, \
        .flags = (_flags), }; \
    if (pinfxn && !(pinfxn)(&infostruct_ ## _pin)) return;

    PIN_LIST

#undef XPINGROUP
#undef XPIN
}
