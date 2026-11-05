.PHONY: all resources btcnf dc clean

PYTHON = $(shell which python3)
PSPDEV = $(shell psp-config --pspdev-path)
BUILDTOOLS = $(PSPDEV)/share/psp-cfw-sdk/build-tools
RESOURCE_DIR = tmp
ARK_DIR = $(RESOURCE_DIR)/ARK
LIBS_DIR = $(RESOURCE_DIR)/LIBS
ARK5_ARCHIVE = $(RESOURCE_DIR)/ARK-5.zip
ARK5_URL = https://github.com/PSP-Arkfive/ARK-5/releases/download/latest/ARK-5.zip
CUSTOMIPL_ARCHIVE = $(RESOURCE_DIR)/CustomIPL.zip
CUSTOMIPL_URL = https://github.com/PSP-Arkfive/CustomIPL/releases/download/latest/CustomIPL.zip
LIBS_ARCHIVE = $(RESOURCE_DIR)/LIBS.zip
LIBS_URL = https://github.com/pspdev/psp-dynamic-libraries/releases/download/latest/LIBS.zip


all: resources btcnf dc
	mkdir -p dist/DC10
	$(PYTHON) $(BUILDTOOLS)/pack/pack.py -p dist/DC10/DC10.ARK packlist.txt -s
	cp Installer/EBOOT.PBP dist/DC10/
	cp $(ARK_DIR)/* dist/DC10/
	cp $(LIBS_DIR)/idsregeneration.prx $(LIBS_DIR)/intraFont-vlf.prx $(LIBS_DIR)/iop.prx $(LIBS_DIR)/ipl_update.prx $(LIBS_DIR)/lflash_fdisk.prx $(LIBS_DIR)/libpsardumper.prx $(LIBS_DIR)/pspdecrypt.prx $(LIBS_DIR)/usbdevice.prx $(LIBS_DIR)/vlf.prx dist/DC10/
	# DAM
	cp -r MagicMemoryCreator dist/
	mkdir dist/MagicMemoryCreator/TM/DCARK/ARK_01234
	mkdir -p dist/MagicMemoryCreator/TM/DCARK/vsh/module/
	cp btcnf/*.bin dist/MagicMemoryCreator/TM/DCARK/kd/
	cp TimeMachine/tmctrl.prx dist/MagicMemoryCreator/TM/DCARK/
	cp DCManager/dcman.prx dist/MagicMemoryCreator/TM/DCARK/kd/
	cp VUnbricker/resurrection.prx dist/MagicMemoryCreator/TM/DCARK/vsh/module/
	cp $(LIBS_DIR)/vlf.prx dist/MagicMemoryCreator/TM/DCARK/vsh/module/
	cp $(LIBS_DIR)/intraFont-vlf.prx dist/MagicMemoryCreator/TM/DCARK/vsh/module/intrafont.prx
	cp $(LIBS_DIR)/idsregeneration.prx dist/MagicMemoryCreator/TM/DCARK/kd/
	cp $(LIBS_DIR)/iop.prx dist/MagicMemoryCreator/TM/DCARK/kd/
	cp $(LIBS_DIR)/ipl_update.prx dist/MagicMemoryCreator/TM/DCARK/kd/
	cp $(LIBS_DIR)/lflash_fdisk.prx dist/MagicMemoryCreator/TM/DCARK/kd/
	cp $(LIBS_DIR)/pspdecrypt.prx dist/MagicMemoryCreator/TM/DCARK/kd/
	cp $(LIBS_DIR)/usbdevice.prx dist/MagicMemoryCreator/TM/DCARK/kd/
	$(PYTHON) $(BUILDTOOLS)/pack/pack.py -e $(ARK_DIR)/CIPL.ARK dist/MagicMemoryCreator/TM/DCARK/
	$(PYTHON) $(BUILDTOOLS)/pack/pack.py -e $(ARK_DIR)/FLASH0.ARK dist/MagicMemoryCreator/TM/DCARK/kd/
	cp dist/MagicMemoryCreator/TM/DCARK/msipl.old dist/MagicMemoryCreator/TM/DCARK/payload_01g.bin
	cp dist/MagicMemoryCreator/TM/DCARK/msipl.old dist/MagicMemoryCreator/TM/DCARK/payload_02g.bin
	cp dist/MagicMemoryCreator/TM/DCARK/ipl_01g.bin dist/MagicMemoryCreator/TM/DCARK/nandipl_01g.bin
	cp dist/MagicMemoryCreator/TM/DCARK/ipl_02g.bin dist/MagicMemoryCreator/TM/DCARK/nandipl_02g.bin

resources:
	mkdir -p $(ARK_DIR) $(LIBS_DIR)
	curl --fail --location --silent --show-error "$(ARK5_URL)" -o "$(ARK5_ARCHIVE)"
	unzip -p "$(ARK5_ARCHIVE)" FLASH0.ARK > "$(ARK_DIR)/FLASH0.ARK"
	curl --fail --location --silent --show-error "$(CUSTOMIPL_URL)" -o "$(CUSTOMIPL_ARCHIVE)"
	unzip -p "$(CUSTOMIPL_ARCHIVE)" CustomIPL/CIPL.ARK > "$(ARK_DIR)/CIPL.ARK"
	curl --fail --location --silent --show-error "$(LIBS_URL)" -o "$(LIBS_ARCHIVE)"
	unzip -o "$(LIBS_ARCHIVE)" -d "$(LIBS_DIR)"

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
	rm -rf $(RESOURCE_DIR)
