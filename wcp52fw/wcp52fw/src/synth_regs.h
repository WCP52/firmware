#ifndef _SYNTH_REGS
#define _SYNTH_REGS 1

//3 byte address
#define FR1_ADDR 0x01
#define FR1_VCOGAIN_I 0
#define FR1_VCOGAIN_B 7
#define FR1_CHARGE_PUMP_CTRL_I 0
#define FR1_CHARGE_PUMP_CTRL_B 0
#define FR1_PLLRATIO_I 0
#define FR1_PLLRATIO_B 2

//3 byte address
#define CFR_ADDR 0x03

//DAC Full scale current control should default 11
#define CFR_DAC_FS_I 1
#define CFR_DAC_FS_B 0

//4 byte address
#define CFTW0_ADDR 0x04




#endif /* _SYNTH_REGS */