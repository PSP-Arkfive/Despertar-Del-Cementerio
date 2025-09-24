
all:
	$(Q)$(MAKE) -C DCManager
	$(Q)$(MAKE) -C CustomIPL msipl
	$(Q)$(MAKE) -C TimeMachine/TMCtrl660/rebootex
	$(Q)$(MAKE) -C TimeMachine/TMCtrl660
	$(Q)$(MAKE) -C VUnbricker
	$(Q)$(MAKE) -C Installer