#ifndef CONF_BOARD_H_INCLUDED
#define CONF_BOARD_H_INCLUDED

#define SPI_MASTER_BASE      SPI

#define ADC_CLOCK       6400000

#define PIN_LIST \
XPINGROUP("Pins for optional FTDI FT230")\
XPIN(GPIO_nSLEEP,       PA0,    PIO_INPUT,              "USB #SLEEP indication")\
XPIN(GPIO_nRTS,         PA1,    PIO_INPUT | PIO_PULLUP, "UART nRTS")\
XPIN(GPIO_nCTS,         PC16,   PIO_OUTPUT_1,           "UART nCTS")\
XPIN(GPIO_UART_RX,      PA9,    PIO_INPUT | PIO_PULLUP, "UART RX")\
XPIN(GPIO_UART_TX,      PA10,   PIO_OUTPUT_1,           "UART TX")\
XPINGROUP("Board management")\
XPIN(GPIO_LED0,         PA11,   PIO_OUTPUT_1,           "Debug LED 0")\
XPIN(GPIO_LED1,         PC2,    PIO_OUTPUT_0,           "Debug LED 1")\
XPIN(GPIO_LED2,         PC12,   PIO_OUTPUT_0,           "Debug LED 2")\
XPIN(GPIO_LED3,         PC13,   PIO_OUTPUT_0,           "Debug LED 3")\
XPIN(GPIO_PWRDN,        PC17,   PIO_OUTPUT_0,           "Power supply control")\
XPINGROUP("Instrument control")\
XPIN(GPIO_CHANSEL,      PA2,    PIO_OUTPUT_0,           "Input channel select")\
XPIN(GPIO_ATTEN,        PC14,   PIO_OUTPUT_0,           "Output attenuator enable")\
XPIN(GPIO_LEVEL,        PA20,   PIO_INPUT,              "Main analog dB level input")\
XPINGROUP("AD9958 interface")\
XPIN(GPIO_DDS_SYNCIO,   PA5,    PIO_OUTPUT_0,           "Resets IO interface")\
XPIN(GPIO_DDS_nCS,      PA6,    PIO_OUTPUT_1,           "SPI chip select (active low)")\
XPIN(GPIO_DDS_MRST,     PA31,   PIO_OUTPUT_1,           "DDS master reset")\
XPIN(GPIO_DDS_IOUPDATE, PB2,    PIO_OUTPUT_0,           "SPI register control")\
XPIN(GPIO_DDS_PWRDN,    PC30,   PIO_OUTPUT_1,           "DDS powerdown")\
XPIN(GPIO_MISO,         PA12,   PIO_PERIPH_A,           "SPI MISO")\
XPIN(GPIO_MOSI,         PA13,   PIO_PERIPH_A,           "SPI MOSI")\
XPIN(GPIO_SCK,          PA14,   PIO_PERIPH_A,           "SPI SCK")\
XPIN(GPIO_DDS_SDIO1,    PA26,   PIO_INPUT | PIO_PULLUP, "Unused DDS SDIO1 pin")\
XPINGROUP("Front panel I2C")\
XPIN(GPIO_SDA,          PA3,    PIO_INPUT,              "I2C data")\
XPIN(GPIO_SCL,          PA4,    PIO_INPUT,              "I2C clock")\


// Generate the basic pin IDs
#define XPINGROUP(grp)
#define XPIN(name, pin, flags, descr) static const int32_t name = PIO_ ## pin ## _IDX;
PIN_LIST
#undef XPIN
#undef XPINGROUP

#endif /* CONF_BOARD_H_INCLUDED */
