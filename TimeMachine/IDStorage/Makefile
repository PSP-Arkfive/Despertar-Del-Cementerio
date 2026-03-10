PSPSDK=$(shell psp-config --pspsdk-path)
CFWSDK = $(PSPDEV)/share/psp-cfw-sdk

TARGET = idstorage

OBJS = \
	main.o \
	
all: $(TARGET).prx

INCDIR = INCDIR = $(PSPSDK)/include $(PSPSDK)/include/iplsdk
CFLAGS = -std=c99 -Os -G0 -Wall -fno-pic -fshort-wchar

CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

BUILD_PRX = 1
PRX_EXPORTS = exports.exp

USE_KERNEL_LIBC=1
USE_KERNEL_LIBS=1

LIBDIR = 
LDFLAGS = -nostartfiles
LIBS = -lidstorage_ms -liplsdk -lansic

PSP_FW_VERSION = 660

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

clean:
	$(Q)rm -rf *.o *.elf *.prx exports.o exports.c