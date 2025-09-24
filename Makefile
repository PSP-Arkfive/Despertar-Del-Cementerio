.PHONY: all dependencies dc clean

ARKSDK = $(CURDIR)/CFW/Libs/ark-dev-sdk
BOOTLOADEX = $(CURDIR)/CFW/Libs/BootLoadEx
CIPLDIR = $(CURDIR)/CustomIPL


all: dependencies dc

dependencies:
	$(Q)$(MAKE) -C CFW full
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX="$(BOOTLOADEX)" -C CustomIPL
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX="$(BOOTLOADEX)" -C TimeMachine/TMCtrl660/rebootex
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX="$(BOOTLOADEX)" -C TimeMachine/TMCtrl660

dc:
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" -C DCManager
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" CIPLDIR="$(CIPLDIR)" -C VUnbricker
#	$(Q)$(MAKE) -C Installer

clean:
	$(Q)$(MAKE) -C CFW fullclean
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX="$(BOOTLOADEX)" -C CustomIPL clean
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX="$(BOOTLOADEX)" -C TimeMachine/TMCtrl660/rebootex clean
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX="$(BOOTLOADEX)" -C TimeMachine/TMCtrl660 clean
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" -C DCManager clean
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" CIPLDIR="$(CIPLDIR)" -C VUnbricker clean
