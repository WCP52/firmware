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

DEP_FILES = $(patsubst %.c,%.d,${SOURCES} ${ASF_SOURCES}) # used by 'clean'

PROG_FILE ?= firmware.elf

###################################################
# Buildchain tools

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

###################################################
# Platform flags

ARM_CFLAGS = \
	-mthumb -DARM_MATH_CM4=true -mcpu=cortex-m4 \
	-mlong-calls -D__SAM4S4C__ -DUDD_ENABLE \
	-DBOARD=USER_BOARD \

ARM_LDFLAGS = -Wl,--entry=Reset_Handler -Wl,--cref -mcpu=cortex-m4 -mthumb \
		  -D=__sam4s4c__ --specs=nosys.specs -pipe -Wl,--gc-sections -Wl,-T \
		  src/ASF/flash.ld -Wl,-Map=flash.map,--cref

###################################################
# Main build flags

LANG_CFLAGS = -Wall -std=c99 -g -fdata-sections -ffunction-sections
${OBJECTS}: EXTRA_CFLAGS := -Wextra #-Werror

###################################################
# Library flags

LDFLAGS = -lm
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
	-Isrc/ASF/sam/boards \
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

CFLAGS = ${INCLUDES} ${ARM_CFLAGS} ${LANG_CFLAGS} ${EXTRA_CFLAGS}

.PHONY: all clean program size_prof

all: firmware.hex

%.o: %.c
	@echo [CC] $<
	@${CC} ${CPPFLAGS} ${CFLAGS} -c $< -o $@

firmware.hex: firmware.elf
	@echo "[OBJCOPY] elf -> hex"
	@${OBJCOPY} -O ihex $< $@

firmware.elf: ${ASF_OBJECTS} ${OBJECTS}
	@echo [LD]
	@${CC} ${ARM_LDFLAGS} ${LDFLAGS} ${ASF_OBJECTS} ${OBJECTS} -lc -lgcc -lnosys -o $@
	@${SIZE} $@

clean:
	@for i in ${ASF_OBJECTS} ${OBJECTS} ${DEP_FILES} firmware.elf firmware.hex; do \
		echo [RM] $$i; \
		rm -f "$$i"; \
	done

program: firmware.hex jlink-script.jlink
	#JLinkExe -device ATSAM4S4C -CommanderScript jlink-script.jlink
	# -c 'at91sam4 gpnvm set 1'
	-openocd -f flash.cfg -c init -c halt -c "program ${PROG_FILE} verify" -c reset -c exit

size_prof: firmware.elf
	nm -CSr --size-sort $< | less
