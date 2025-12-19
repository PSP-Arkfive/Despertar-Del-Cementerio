.PHONY: all btcnf dc clean

PSPDEV = $(shell psp-config --pspdev-path)
ARKSDK = $(PSPDEV)/share/ark-dev-sdk
BOOTLOADEX = $(CURDIR)/CFW/Libs/BootLoadEx
CIPLDIR = $(CURDIR)/CustomIPL
MODDIR = $(CURDIR)/ExternalModules


all: btcnf dc

btcnf:
	$(PYTHON) $(ARKSDK)/build-tools/btcnf.py build btcnf/pspbtcnf_dc.txt
	$(PYTHON) $(ARKSDK)/build-tools/btcnf.py build btcnf/pspbtcnf_02g_dc.txt
	$(PYTHON) $(ARKSDK)/build-tools/btcnf.py build btcnf/pspbtcnf_03g_dc.txt
	$(PYTHON) $(ARKSDK)/build-tools/btcnf.py build btcnf/pspbtcnf_04g_dc.txt
	$(PYTHON) $(ARKSDK)/build-tools/btcnf.py build btcnf/pspbtcnf_05g_dc.txt
	$(PYTHON) $(ARKSDK)/build-tools/btcnf.py build btcnf/pspbtcnf_07g_dc.txt
	$(PYTHON) $(ARKSDK)/build-tools/btcnf.py build btcnf/pspbtcnf_09g_dc.txt
	$(PYTHON) $(ARKSDK)/build-tools/btcnf.py build btcnf/pspbtcnf_11g_dc.txt

dc:
	$(Q)$(MAKE) -C DCManager
	$(Q)$(MAKE) CIPLDIR="$(CIPLDIR)" -C VUnbricker
	$(Q)$(MAKE) CIPLDIR="$(CIPLDIR)" MODDIR="$(MODDIR)" -C Installer

clean:
	$(Q)$(MAKE) -C DCManager clean
	$(Q)$(MAKE) CIPLDIR="$(CIPLDIR)" -C VUnbricker clean
	$(Q)$(MAKE) -C Installer clean
	$(Q)rm -f btcnf/*.bin
