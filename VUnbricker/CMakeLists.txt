add_prx_module(resurrection exports.exp)

target_sources(resurrection PRIVATE 
   crt0.c
   main.c
   src/flash_format.c
   src/idstools.c
   src/install.c
   src/nandoperations.c
)

target_compile_options(resurrection PRIVATE -std=c99 -O2 -G0 -Wall -fshort-wchar -fno-pic -mno-check-zero-division)
target_include_directories(resurrection PRIVATE include ${CFWSDK}/include)
target_link_directories(resurrection PRIVATE ${CFWSDK}/libs)
target_link_options(resurrection PRIVATE -nostdlib)
target_link_libraries(resurrection PRIVATE
   psprtc 
   pspreg
   pspwlan
   psppower
   pspuser
	vlfgui
   vlfgu
   vlfutils
   vlflibc
	pspsystemctrl_user
   pspkubridge
	pspdcman
   pspdecrypt
   pspiplupdate
   idsregeneration
   pspdebug
   pspdisplay
   pspge
   pspctrl
   pspnet
   pspnet_apctl
   pspmodinfo
)
