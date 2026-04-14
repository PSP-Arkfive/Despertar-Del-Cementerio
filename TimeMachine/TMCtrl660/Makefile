PSPSDK=$(shell psp-config --pspsdk-path)
CFWSDK = $(PSPDEV)/share/psp-cfw-sdk
PY = $(shell which python3)

TARGET = tmctrl

OBJS = \
	main.o \
	flashemu.o \
	
all: cptbl.h $(TARGET).prx tmctrl.h

tmctrl.h: $(TARGET).prx
	$(Q)bin2c $(TARGET).prx tmctrl.h tmctrl

cptbl.h:
	$(Q)dd if=../../Resources/cptbl.dat of=cptbl.dat bs=2048 conv=sync
	$(Q)$(PY) $(CFWSDK)/build-tools/ciso/ciso.py -c 1 cptbl.dat cptbl.cso
	$(Q)bin2c cptbl.cso cptbl.h cptbl
	$(Q)rm cptbl.dat cptbl.cso

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
LIBS = -lcisoread -lpspsystemctrl_kernel -lpspsemaphore_660

PSP_FW_VERSION = 660

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

clean:
	$(Q)rm -rf *.o *.elf *.prx exports.o exports.c tmctrl.h cptbl.h