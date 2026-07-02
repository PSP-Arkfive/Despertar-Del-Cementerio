PSPSDK=$(shell psp-config --pspsdk-path)
PSPDEV = $(shell psp-config --pspdev-path)
CFWSDK = $(PSPDEV)/share/psp-cfw-sdk

TARGET = arkdcinstaller
OBJS = \
	crt0.o \
	main.o \


CFLAGS = -std=c99 -O2 -G0 -Wall -fshort-wchar -fno-pic -mno-check-zero-division 
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS) -c


INCDIR = ./
LIBDIR = /usr/local/pspdev/psp/sdk/lib/
LDFLAGS = -nostartfiles
LIBS = -ltinyfont -lcolordebugger -lpspsystemctrl_user -lpspkubridge -lpsppower -lvlfgui -lvlfgu -lvlfutils -lvlflibc -lpspdecrypt -lpsppsar -lpspnet_apctl -lpsphttp -lpspnet_resolver -lpspnet_inet -lpspnet -lpspsdk
PSP_FW_VERSION = 660

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = ARK DC Installer
PSP_EBOOT_ICON = res/cementerio.png
PSP_EBOOT_ICON1 = res/icon1.pmf
PSP_EBOOT_SND0 = res/snd0.at3
PSP_EBOOT_PIC0 = res/despertar.png
BUILD_PRX = 1

all: $(TARGET).prx

include $(PSPSDK)/lib/build.mak
