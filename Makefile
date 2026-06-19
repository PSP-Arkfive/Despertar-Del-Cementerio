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
	cp Resources/LIBS/* dist/DC10/
	# Time machine stuff for DAM
	cp TimeMachine/tmctrl.prx MagicMemoryCreator/TM/DCARK/
	## FLASH0 for DAM
	curl -OJL 'https://github.com/PSP-Arkfive/ARK-5/releases/download/latest/ARK-5.zip'
	unzip ARK-5.zip
	rm FLASH0.ARK
	mv flash0 FLASH0
	cp FLASH0/*.prx MagicMemoryCreator/TM/DCARK/kd/
	# btcnf files for DAM
	cp -r btcnf/pspbtcnf_*dc.bin MagicMemoryCreator/TM/DCARK/kd/
	# CIPL files for DAM
	curl -OJL 'https://github.com/PSP-Arkfive/CustomIPL/releases/download/latest/CustomIPL.zip'
	unzip CustomIPL.zip
	mv CustomIPL CIPL
	cd CIPL && $(PYTHON) $(BUILDTOOLS)/pack/pack.py -e CIPL.ARK
	cp CIPL/*.prx MagicMemoryCreator/TM/DCARK/
	# Copy intrafont to DAM
	cp Resources/LIBS/intraFont-vlf.prx MagicMemoryCreator/TM/DCARK/vsh/module/intrafont.prx
	# Copy vlf to DAM
	cp Resources/LIBS/vlf.prx MagicMemoryCreator/TM/DCARK/vsh/module/
	# Copy idregen to DAM
	cp Resources/LIBS/idsregeneration.prx MagicMemoryCreator/TM/DCARK/kd/
	# Copy iop to DAM
	cp Resources/LIBS/iop.prx MagicMemoryCreator/TM/DCARK/kd/
	# Copy ipl_update to DAM
	cp Resources/LIBS/ipl_update.prx MagicMemoryCreator/TM/DCARK/kd/
	# Copy lflash_fdisk to DAM
	cp Resources/LIBS/lflash_fdisk.prx MagicMemoryCreator/TM/DCARK/kd/
	# Copy usbdevice to DAM
	cp Resources/LIBS/usbdevice.prx MagicMemoryCreator/TM/DCARK/kd/
	# Copy msipl.bin to DAM
	cp CIPL/msipl.bin MagicMemoryCreator/TM/DCARK/
	cp CIPL/msipl.raw MagicMemoryCreator/TM/DCARK/
	cp CIPL/msipl.bin MagicMemoryCreator/
	# Copy MagicMemoryCreator
	cp -r MagicMemoryCreator/ dist/

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
	rm -rf MagicMemoryCreator/TM/DCARK/vsh/module/vlf.prx
	rm -rf MagicMemoryCreator/TM/DCARK/codepage/
	rm -rf MagicMemoryCreator/TM/DCARK/data/
	rm -rf MagicMemoryCreator/TM/DCARK/dic/
	rm -rf MagicMemoryCreator/TM/DCARK/font/
	rm -rf MagicMemoryCreator/TM/DCARK/kd/resource/
	rm -rf MagicMemoryCreator/TM/DCARK/vsh/etc/
	rm -rf MagicMemoryCreator/TM/DCARK/vsh/resource/
	rm -rf ARK-5.zip CustomIPL.zip FLASH0 CIPL
	rm -rf MagicMemoryCreator/TM/DCARK/msipl.raw
