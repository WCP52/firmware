SOURCES = \
	src/acquisition.c \
	src/main.c \
	src/scpi.c \
	src/scpi-def.c \
	src/scpi-test.c \
	src/synth.c \
	src/usb-functions.c \
	src/util.c \
	$(shell find ../scpi-parser/libscpi/src -name '*.c')

ASF_SOURCES = $(shell find src/ASF -name '*.c')

ASF_OBJECTS = $(patsubst %.c,%.o,${ASF_SOURCES})
OBJECTS = $(patsubst %.c,%.o,${SOURCES})

###################################################
# Compiler setups
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = ${INCLUDES} ${ARM_CFLAGS} ${LANG_CFLAGS} ${EXTRA_CFLAGS}
ARM_CFLAGS = -mthumb -D__SAM4S16C__ -DBOARD=SAM4S_XPLAINED -DARM_MATH_CM4=true \
			 -Dprintf=iprintf -DUDD_ENABLE -fdata-sections -ffunction-sections \
			 -mlong-calls -mcpu=cortex-m4
LANG_CFLAGS = -Wall -std=gnu99
${OBJECTS}: EXTRA_CFLAGS := -Wextra -Werror
${ASF_OBJECTS}: EXTRA_CFLAGS := -Wno-unused-but-set-variable

INCLUDES = \
	-I../scpi-parser/libscpi/inc \
	-Isrc \
	-Isrc/ASF/common/boards \
	-Isrc/ASF/common/services/clock \
	-Isrc/ASF/common/services/delay \
	-Isrc/ASF/common/services/gpio \
	-Isrc/ASF/common/services/ioport \
	-Isrc/ASF/common/services/serial \
	-Isrc/ASF/common/services/serial/sam_uart \
	-Isrc/ASF/common/services/sleepmgr \
	-Isrc/ASF/common/services/spi \
	-Isrc/ASF/common/services/usb \
	-Isrc/ASF/common/services/usb/class/cdc \
	-Isrc/ASF/common/services/usb/class/cdc/device \
	-Isrc/ASF/common/services/usb/udc \
	-Isrc/ASF/common/utils \
	-Isrc/ASF/common/utils/stdio/stdio_serial \
	-Isrc/ASF/sam/applications/getting-started \
	-Isrc/ASF/sam/applications/getting-started/sam4s16c_sam4s_xplained \
	-Isrc/ASF/sam/applications/getting-started/sam4s16c_sam4s_xplained/gcc \
	-Isrc/ASF/sam/boards \
	-Isrc/ASF/sam/boards/sam4s_xplained \
	-Isrc/ASF/sam/drivers/adc \
	-Isrc/ASF/sam/drivers/pio \
	-Isrc/ASF/sam/drivers/pmc \
	-Isrc/ASF/sam/drivers/spi \
	-Isrc/ASF/sam/drivers/uart \
	-Isrc/ASF/sam/drivers/udp \
	-Isrc/ASF/sam/drivers/usart \
	-Isrc/ASF/sam/utils \
	-Isrc/ASF/sam/utils/cmsis/sam4s/include \
	-Isrc/ASF/sam/utils/cmsis/sam4s/source/templates \
	-Isrc/ASF/sam/utils/header_files \
	-Isrc/ASF/sam/utils/preprocessor \
	-Isrc/ASF/thirdparty/CMSIS/Include \
	-Isrc/ASF/thirdparty/CMSIS/Lib/GCC \
	-Isrc/config \


.PHONY: all clean

all: firmware.hex

%.o: %.c
	@echo [CC] $<
	@${CC} ${CPPFLAGS} ${CFLAGS} -c $< -o $@

firmware.hex: firmware.elf
	@echo "[OBJCOPY] elf -> hex"
	@${OBJCOPY} -O ihex $< $@

firmware.elf: ${ASF_OBJECTS} ${OBJECTS}
	@echo [LD]
	@${CC} -Wl,--entry=Reset_Handler -Wl,--cref ${ARM_CFLAGS} ${LANG_CFLAGS} \
		--specs=nosys.specs -Wl,--gc-sections -Wl,-T src/ASF/flash.ld -Wl,-Map=firmware.map,--cref \
		-Lsrc/ASF/thirdpart/CMSIS/Lib/GCC ${ASF_OBJECTS} ${OBJECTS} -lc -lgcc -lnosys -lm -o $@

clean:
	@for i in ${ASF_OBJECTS} ${OBJECTS} firmware.elf firmware.hex; do \
		echo [RM] $$i; \
		rm -f "$$i"; \
	done
