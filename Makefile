.PHONY: all dependencies btcnf dc clean

ARKSDK = $(CURDIR)/CFW/Libs/ark-dev-sdk
BOOTLOADEX = $(CURDIR)/CFW/Libs/BootLoadEx
CIPLDIR = $(CURDIR)/CustomIPL
MODDIR = $(CURDIR)/ExternalModules


all: dependencies btcnf dc

dependencies:
	$(Q)$(MAKE) -C CFW full
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX="$(BOOTLOADEX)" -C CustomIPL
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX="$(BOOTLOADEX)" -C TimeMachine/TMCtrl660/rebootex
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX="$(BOOTLOADEX)" -C TimeMachine/TMCtrl660
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" -C ExternalModules/IPL_Updater
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" -C ExternalModules/IOPrivileged
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" -C ExternalModules/idStorageRegen

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
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" -C DCManager
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" CIPLDIR="$(CIPLDIR)" -C VUnbricker
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" CIPLDIR="$(CIPLDIR)" MODDIR="$(MODDIR)" -C Installer

clean:
	$(Q)$(MAKE) -C CFW fullclean
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX="$(BOOTLOADEX)" -C CustomIPL clean
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX="$(BOOTLOADEX)" -C TimeMachine/TMCtrl660/rebootex clean
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX="$(BOOTLOADEX)" -C TimeMachine/TMCtrl660 clean
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" -C DCManager clean
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" CIPLDIR="$(CIPLDIR)" -C VUnbricker clean
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" -C Installer clean
	$(Q)rm -f btcnf/*.bin
