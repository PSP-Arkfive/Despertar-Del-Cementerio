PSPSDK=$(shell psp-config --pspsdk-path)


TARGET = resurrection
OBJS = crt0.o main.o imports.o \
	src/install.o \
	src/nandoperations.o \
	src/flash_format.o \
	src/idstools.o \
	libs/DcManager.o \
	libs/pspDecrypt.o \
	libs/pspIplUpdate.o \
	libs/idsRegeneration.o \

INCDIR = include
CFLAGS = -std=c99 -O2 -G0 -Wall -fshort-wchar -fno-pic -mno-check-zero-division
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS) -c

LIBDIR = $(ARKSDK)/libs
LDFLAGS = -nostartfiles
LIBS = -lpsprtc -lvlfgui -lvlfgu -lvlfutils -lvlflibc -lpspreg -lpspsystemctrl_user -lpspkubridge -lpspwlan -lpsppower -lpspkernel

PSP_FW_VERSION = 661
PRX_EXPORTS = exports.exp

BUILD_PRX = 1

include $(PSPSDK)/lib/build.mak
