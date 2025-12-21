PSPSDK=$(shell psp-config --pspsdk-path)
ARKSDK = ../../ark-dev-sdk

TARGET = tmctrl150

OBJS = \
	main.o \
	flashemu.o \
	
all: $(TARGET).prx tmctrl.h

tmctrl.h: $(TARGET).prx
	$(Q)bin2c $(TARGET).prx tmctrl.h tmctrl

INCDIR = $(ARKSDK)/include
CFLAGS = -std=c99 -Os -G0 -Wall -fno-pic -fshort-wchar
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
PRX_EXPORTS = exports.exp

USE_KERNEL_LIBC=1
USE_KERNEL_LIBS=1

LIBDIR = $(ARKSDK)/libs
LDFLAGS = -nostartfiles
LIBS = -lpspsystemctrl_kernel

PSP_FW_VERSION = 150

include $(PSPSDK)/lib/build.mak

clean:
	$(Q)rm -rf *.o *.elf *.prx *.bin tmctrl.h