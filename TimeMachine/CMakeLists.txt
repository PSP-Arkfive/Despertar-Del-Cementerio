set(TMCTRL_MOD_LIBS -nostdlib pspsystemctrl_kernel pspmodinfo pspsdk pspkernel)
add_compile_options(-std=c99 -Os -G0 -Wall -fno-pic -fshort-wchar)

add_subdirectory(TMCtrl150)
add_subdirectory(TMCtrl660)
add_subdirectory(IDStorage)