ARKSDK = $(CURDIR)/CFW/Libs/ark-dev-sdk
BOOTLOADEX = $(CURDIR)/CFW/Libs/BootLoadEx


all:
	$(Q)$(MAKE) -C CFW full
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX="$(BOOTLOADEX)" -C CustomIPL msipl
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX="$(BOOTLOADEX)" -C TimeMachine/TMCtrl660/rebootex
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX="$(BOOTLOADEX)" -C TimeMachine/TMCtrl660
#	$(Q)$(MAKE) -C DCManager
#	$(Q)$(MAKE) -C VUnbricker
#	$(Q)$(MAKE) -C Installer

clean:
	$(Q)$(MAKE) -C CFW fullclean
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX="$(BOOTLOADEX)" -C CustomIPL clean
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX="$(BOOTLOADEX)" -C TimeMachine/TMCtrl660/rebootex clean
	$(Q)$(MAKE) ARKSDK="$(ARKSDK)" BOOTLOADEX="$(BOOTLOADEX)" -C TimeMachine/TMCtrl660 clean
	