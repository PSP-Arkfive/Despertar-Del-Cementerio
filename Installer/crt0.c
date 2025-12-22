#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <pspsdk.h>
#include <pspkernel.h>
#include <pspdisplay.h>

#include <kubridge.h>
#include <vlf.h>
#include <screenprinter.h>

extern char boot_path[256];
extern int app_main(int argc, char *argv[]);

static void SimpleErrorExit(int milisecs, char* errortext, int errorcode){
    initScreen(&sceDisplaySetFramebuf);
    PRTSTR2("ERROR: %s (%p)", errortext, errorcode);
    sceKernelDelayThread(milisecs*1000);
    sceKernelExitGame();
}

int start_thread(SceSize args, void *argp)
{
    SceUID mod;
    char *path = (char *)argp;
    int last_trail = -1;
    int i = 0;

    if (path)
    {
        for (i = 0; path[i]; i++)
        {
        	boot_path[i] = path[i];
        	if (path[i] == '/')
        		last_trail = i;
        }
    }
    
    boot_path[i] = 0;

    if (last_trail >= 0)
        boot_path[last_trail] = 0;

    sceIoChdir(boot_path);

    // load intraFont-vlf
    mod = kuKernelLoadModule("intraFont-vlf.prx", 0, NULL);
    if (mod < 0) mod = kuKernelLoadModule("ms0:/PSP/LIBS/intraFont-vlf.prx", 0, NULL); // retry LIBS folder
    if (mod < 0) SimpleErrorExit(5000, "Filed to load module intraFont-vlf.prx", mod);

    // start intraFont-vlf
    mod = sceKernelStartModule(mod, args, argp, NULL, NULL);
    if (mod < 0) SimpleErrorExit(5000, "Filed to start module intraFont-vlf.prx", mod);

    // load vlf
    mod = kuKernelLoadModule("vlf.prx", 0, NULL);
    if (mod < 0) mod = kuKernelLoadModule("ms0:/PSP/LIBS/vlf.prx", 0, NULL); // retry LIBS folder
    if (mod < 0) SimpleErrorExit(5000, "Filed to load module vlf.prx", mod);

    // start vlf
    mod = sceKernelStartModule(mod, args, argp, NULL, NULL);
    if (mod < 0) SimpleErrorExit(5000, "Filed to start module vlf.prx", mod);
    
    vlfGuiInit(-1, app_main);
    
    return sceKernelExitDeleteThread(0);
}

int _start(SceSize args, void *argp)
{

    SceUID thid = sceKernelCreateThread("start_thread", start_thread, 0x10, 0x4000, 0, NULL);

    if (thid < 0)
        return thid;

    sceKernelStartThread(thid, args, argp);
    
    return 0;
}
