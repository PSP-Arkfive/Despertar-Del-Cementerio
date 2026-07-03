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
	# DAM
	cp -r MagicMemoryCreator dist/
	mkdir dist/MagicMemoryCreator/TM/DCARK/ARK_01234
	mkdir -p dist/MagicMemoryCreator/TM/DCARK/vsh/module/
	cp btcnf/*.bin dist/MagicMemoryCreator/TM/DCARK/kd/
	cp TimeMachine/tmctrl.prx dist/MagicMemoryCreator/TM/DCARK/
	cp DCManager/dcman.prx dist/MagicMemoryCreator/TM/DCARK/kd/
	cp VUnbricker/resurrection.prx dist/MagicMemoryCreator/TM/DCARK/vsh/module/
	cp Resources/LIBS/vlf.prx dist/MagicMemoryCreator/TM/DCARK/vsh/module/
	cp Resources/LIBS/intraFont-vlf.prx dist/MagicMemoryCreator/TM/DCARK/vsh/module/intrafont.prx
	cp Resources/LIBS/idsregeneration.prx dist/MagicMemoryCreator/TM/DCARK/kd/
	cp Resources/LIBS/iop.prx dist/MagicMemoryCreator/TM/DCARK/kd/
	cp Resources/LIBS/ipl_update.prx dist/MagicMemoryCreator/TM/DCARK/kd/
	cp Resources/LIBS/lflash_fdisk.prx dist/MagicMemoryCreator/TM/DCARK/kd/
	cp Resources/LIBS/pspdecrypt.prx dist/MagicMemoryCreator/TM/DCARK/kd/
	cp Resources/LIBS/usbdevice.prx dist/MagicMemoryCreator/TM/DCARK/kd/
	$(PYTHON) $(BUILDTOOLS)/pack/pack.py -e Resources/ARK/CIPL.ARK dist/MagicMemoryCreator/TM/DCARK/
	$(PYTHON) $(BUILDTOOLS)/pack/pack.py -e Resources/ARK/FLASH0.ARK dist/MagicMemoryCreator/TM/DCARK/kd/
	cp dist/MagicMemoryCreator/TM/DCARK/msipl.old dist/MagicMemoryCreator/TM/DCARK/payload_01g.bin
	cp dist/MagicMemoryCreator/TM/DCARK/msipl.old dist/MagicMemoryCreator/TM/DCARK/payload_02g.bin
	cp dist/MagicMemoryCreator/TM/DCARK/ipl_01g.bin dist/MagicMemoryCreator/TM/DCARK/nandipl_01g.bin
	cp dist/MagicMemoryCreator/TM/DCARK/ipl_02g.bin dist/MagicMemoryCreator/TM/DCARK/nandipl_02g.bin

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
