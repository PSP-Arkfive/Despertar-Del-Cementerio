#ifndef __FLASHEMU_H__
#define __FLASHEMU_H__

#include <pspiofilemgr_kernel.h>

/** Structure passed to the file functions of the io driver system */
typedef struct SceIoIobFE
{
    int i_flgs;
    int i_unit;
    SceIoDeviceEntry *i_de;
    int d_type;
    void *i_private;
    SceIoCwd *i_cwd;
    SceOff i_fpos;
    SceUID i_thread;
    int dummy;
} SceIoIobFE;

int InstallFlashEmu();
int UninstallFlashEmu();
void PrepareRebootFlashEmu();

#endif
