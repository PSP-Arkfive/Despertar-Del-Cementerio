PSPSDK=$(shell psp-config --pspsdk-path)

TARGET = tmctrl

OBJS = \
	main.o \
	flashemu.o \
	
all: $(TARGET).prx tmctrl.h

tmctrl.h: $(TARGET).prx
	$(Q)bin2c $(TARGET).prx tmctrl.h tmctrl

INCDIR = rebootex
CFLAGS = -std=c99 -Os -G0 -Wall -fno-pic -fshort-wchar

CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
PRX_EXPORTS = exports.exp

USE_KERNEL_LIBC=1
USE_KERNEL_LIBS=1

LIBDIR = 
LDFLAGS = -nostartfiles
LIBS = -lpspsystemctrl_kernel -lpspsemaphore_660

PSP_FW_VERSION = 660

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

clean:
	$(Q)rm -rf *.o *.elf *.prx exports.o exports.c tmctrl.h