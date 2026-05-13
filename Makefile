.PHONY: all btcnf dc clean

PYTHON = $(shell which python3)
PSPDEV = $(shell psp-config --pspdev-path)
BUILDTOOLS = $(PSPDEV)/share/psp-cfw-sdk/build-tools


all: btcnf dc
	$(Q)mkdir -p dist/DC10
	$(PYTHON) $(BUILDTOOLS)/pack/pack.py -p dist/DC10/DC10.ARK packlist.txt -s
	$(Q)cp Installer/EBOOT.PBP dist/DC10/
	$(Q)cp Resources/ARK/* dist/DC10/
	$(Q)cp Resources/LIBS/* dist/DC10/

btcnf:
	$(PYTHON) $(BUILDTOOLS)/btcnf.py build btcnf/pspbtcnf_dc.txt
	$(PYTHON) $(BUILDTOOLS)/btcnf.py build btcnf/pspbtcnf_02g_dc.txt
	$(PYTHON) $(BUILDTOOLS)/btcnf.py build btcnf/pspbtcnf_03g_dc.txt
	$(PYTHON) $(BUILDTOOLS)/btcnf.py build btcnf/pspbtcnf_04g_dc.txt
	$(PYTHON) $(BUILDTOOLS)/btcnf.py build btcnf/pspbtcnf_05g_dc.txt
	$(PYTHON) $(BUILDTOOLS)/btcnf.py build btcnf/pspbtcnf_07g_dc.txt
	$(PYTHON) $(BUILDTOOLS)/btcnf.py build btcnf/pspbtcnf_09g_dc.txt
	$(PYTHON) $(BUILDTOOLS)/btcnf.py build btcnf/pspbtcnf_11g_dc.txt

dc:
	$(Q)$(MAKE) -C Installer
	$(Q)$(MAKE) -C DCManager
	$(Q)$(MAKE) -C VUnbricker
	$(Q)$(MAKE) -C TimeMachine/rebootex
	$(Q)$(MAKE) -C TimeMachine/

clean:
	$(Q)$(MAKE) -C Installer clean
	$(Q)$(MAKE) -C DCManager clean
	$(Q)$(MAKE) -C VUnbricker clean
	$(Q)$(MAKE) -C TimeMachine/rebootex clean
	$(Q)$(MAKE) -C TimeMachine/ clean
	$(Q)rm -f btcnf/*.bin
	$(Q)rm -rf dist
