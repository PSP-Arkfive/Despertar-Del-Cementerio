.PHONY: all btcnf dc clean

PYTHON = $(shell which python3)
PSPDEV = $(shell psp-config --pspdev-path)
BUILDTOOLS = $(PSPDEV)/share/psp-cfw-sdk/build-tools


all: btcnf dc
	mkdir -p dist/DC10
	$(PYTHON) $(BUILDTOOLS)/pack/pack.py -p dist/DC10/DC10.ARK packlist.txt -s
	cp Installer/EBOOT.PBP dist/DC10/
	cp Resources/ARK/* dist/DC10/
	cp Resources/LIBS/* dist/DC10/

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
	$(MAKE) -C Installer
	$(MAKE) -C DCManager
	$(MAKE) -C VUnbricker
	$(MAKE) -C TimeMachine/rebootex
	$(MAKE) -C TimeMachine/

clean:
	$(MAKE) -C Installer clean
	$(MAKE) -C DCManager clean
	$(MAKE) -C VUnbricker clean
	$(MAKE) -C TimeMachine/rebootex clean
	$(MAKE) -C TimeMachine/ clean
	rm -f btcnf/*.bin
	rm -rf dist
