add_prx_module(dcman exports.exp)

target_sources(dcman PRIVATE 
   main.c
   LflashFdisk.S
   imports.S
)

target_compile_options(dcman PRIVATE -O2 -Os -G0 -Wall -fshort-wchar -fno-pic -mno-check-zero-division)
target_include_directories(dcman PRIVATE include ${CFWSDK}/include)
target_link_directories(dcman PRIVATE ${CFWSDK}/libs)
target_link_libraries(dcman PRIVATE
   -nostdlib
   pspnand_driver
   pspsystemctrl_kernel
   psplflash_fatfmt
   pspsemaphore_660
   pspsdk
   pspmodinfo
   pspkernel
)
