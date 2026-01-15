PSPSDK=$(shell psp-config --pspsdk-path)


TARGET = resurrection
OBJS = crt0.o main.o \
	src/install.o \
	src/nandoperations.o \
	src/flash_format.o \
	src/idstools.o \

INCDIR = include
CFLAGS = -std=c99 -O2 -G0 -Wall -fshort-wchar -fno-pic -mno-check-zero-division
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS) -c

LIBDIR = $(CFWSDK)/libs
LDFLAGS = -nostartfiles
LIBS = -lpsprtc -lpspreg -lpspwlan -lpsppower \
	-lvlfgui -lvlfgu -lvlfutils -lvlflibc \
	-lpspsystemctrl_user -lpspkubridge \
	-lpspdcman -lpspdecrypt -lpspiplupdate -lidsregeneration

PSP_FW_VERSION = 661
PRX_EXPORTS = exports.exp

BUILD_PRX = 1

include $(PSPSDK)/lib/build.mak
