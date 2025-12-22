#include <string.h>

#include <cfwmacros.h>
#include <systemctrl.h>
#include <systemctrl_se.h>
#include <bootloadex.h>
#include <bootloadex_ark.h>

#include <fat.h>
#include <syscon.h>

#define REG32(addr) *((volatile uint32_t *)(addr))

BootLoadExConfig bleconfig = {
    .boot_type = TYPE_REBOOTEX,
    .boot_storage = MS_BOOT,
    .extra_io = {
        .psp_io = {
            .tm_path = "/TM/DCARK",
            .FatMount = &MsFatMount,
            .FatOpen = &MsFatOpen,
            .FatRead = &MsFatRead,
            .FatClose = &MsFatClose,
        }
    }
};

// Entry Point
int cfwBoot(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7)
{
    #ifdef DEBUG
    colorDebug(0xff00);
    #endif

    // GPIO enable
    REG32(0xbc10007c) |= 0xc8;
    __asm("sync"::);
    
    syscon_init();
    syscon_ctrl_ms_power(1);

    // Configure
    configureBoot(&bleconfig);

    // scan functions
    findBootFunctions();
    
    // patch sceboot
    patchBootPSP(&UnpackBootConfigArkPSP);
    
    // Forward Call
    return sceBoot(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}
