	.set noreorder

#include "pspimport.s"

	IMPORT_START "sceNand_driver",0x00010000
	IMPORT_FUNC	"sceNand_driver",0xC478C1DE,sceNandReadPagesRawAll
