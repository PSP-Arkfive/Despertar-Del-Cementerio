PSPSDK=$(shell psp-config --pspsdk-path)
PSPDEV = $(shell psp-config --pspdev-path)
ARKSDK = $(PSPDEV)/share/ark-dev-sdk

TARGET = arkdcinstaller
OBJS = \
	crt0.o \
	pspbtcnf_dc.h \
	pspbtcnf_02g_dc.h \
	pspbtcnf_03g_dc.h \
	pspbtcnf_04g_dc.h \
	pspbtcnf_05g_dc.h \
	pspbtcnf_07g_dc.h \
	pspbtcnf_09g_dc.h \
	pspbtcnf_11g_dc.h \
	main.o \
	libs/pspDecrypt.o \
	libs/pspPSAR.o \


INCDIR = ../include $(PSPSDK)/include \
	$(CIPLDIR)/MSIPL/newipl/stage2 \
	$(CIPLDIR)/MSIPL/newipl/stage3 \
	$(CIPLDIR)/NewIPL/

CFLAGS = -std=c99 -O2 -G0 -Wall -fshort-wchar -fno-pic -mno-check-zero-division 
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS) -c

LIBDIR = $(PSPSDK)/lib
LDFLAGS = -nostartfiles
LIBS = -lpspsystemctrl_user -lpspkubridge -lpsppower -lvlfgui -lvlfgu -lvlfutils -lpspsysc_user -lvlflibc

PSP_FW_VERSION = 660

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = ARK DC Installer
PSP_EBOOT_ICON = res/cementerio.png
PSP_EBOOT_PIC0 = res/despertar.png

all: $(TARGET).prx

pspbtcnf_dc.h:
	$(Q)bin2c ../btcnf/pspbtcnf_dc.bin pspbtcnf_dc.h pspbtcnf_dc

pspbtcnf_02g_dc.h:
	$(Q)bin2c ../btcnf/pspbtcnf_02g_dc.bin pspbtcnf_02g_dc.h pspbtcnf_02g_dc

pspbtcnf_03g_dc.h:
	$(Q)bin2c ../btcnf/pspbtcnf_03g_dc.bin pspbtcnf_03g_dc.h pspbtcnf_03g_dc

pspbtcnf_04g_dc.h:
	$(Q)bin2c ../btcnf/pspbtcnf_04g_dc.bin pspbtcnf_04g_dc.h pspbtcnf_04g_dc

pspbtcnf_05g_dc.h:
	$(Q)bin2c ../btcnf/pspbtcnf_05g_dc.bin pspbtcnf_05g_dc.h pspbtcnf_05g_dc

pspbtcnf_07g_dc.h:
	$(Q)bin2c ../btcnf/pspbtcnf_07g_dc.bin pspbtcnf_07g_dc.h pspbtcnf_07g_dc

pspbtcnf_09g_dc.h:
	$(Q)bin2c ../btcnf/pspbtcnf_09g_dc.bin pspbtcnf_09g_dc.h pspbtcnf_09g_dc

pspbtcnf_11g_dc.h:
	$(Q)bin2c ../btcnf/pspbtcnf_11g_dc.bin pspbtcnf_11g_dc.h pspbtcnf_11g_dc

distclean:
	make clean

include $(PSPSDK)/lib/build.mak
