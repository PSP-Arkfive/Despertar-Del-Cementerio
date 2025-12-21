.PHONY: all btcnf dc clean

PSPDEV = $(shell psp-config --pspdev-path)
ARKSDK = $(PSPDEV)/share/ark-dev-sdk


all: btcnf dc
	$(Q)mkdir -p dist/DC10
	$(Q)cp DCManager/dcman.prx dist/DC10/
	$(Q)cp VUnbricker/resurrection.prx dist/DC10/
	$(Q)cp Installer/EBOOT.PBP dist/DC10/
	$(Q)cp TimeMachine/TMCtrl660/*.prx dist/DC10/
	$(Q)cp TimeMachine/TMCtrl150/*.prx dist/DC10/

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
	$(Q)$(MAKE) -C Installer
	$(Q)$(MAKE) -C DCManager
	$(Q)$(MAKE) -C VUnbricker
	$(Q)$(MAKE) -C TimeMachine/TMCtrl660/rebootex
	$(Q)$(MAKE) -C TimeMachine/TMCtrl660
	$(Q)$(MAKE) -C TimeMachine/TMCtrl150

clean:
	$(Q)$(MAKE) -C Installer clean
	$(Q)$(MAKE) -C DCManager clean
	$(Q)$(MAKE) -C VUnbricker clean
	$(Q)$(MAKE) -C TimeMachine/TMCtrl660/rebootex clean
	$(Q)$(MAKE) -C TimeMachine/TMCtrl660 clean
	$(Q)$(MAKE) -C TimeMachine/TMCtrl150 clean
	$(Q)rm -f btcnf/*.bin
	$(Q)rm -rf dist
