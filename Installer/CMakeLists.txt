add_executable(arkdcinstaller)

target_sources(arkdcinstaller PRIVATE
   crt0.c   
   main.c
   pspbtcnf_dc.h
   pspbtcnf_02g_dc.h
   pspbtcnf_03g_dc.h
   pspbtcnf_04g_dc.h
   pspbtcnf_05g_dc.h
   pspbtcnf_07g_dc.h
   pspbtcnf_09g_dc.h
   pspbtcnf_11g_dc.h
)

target_compile_options(arkdcinstaller PRIVATE -O2 -G0 -Wall -fshort-wchar)
target_include_directories(arkdcinstaller PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_options(arkdcinstaller PRIVATE -nostdlib)
target_link_libraries(arkdcinstaller PRIVATE
   screenprinter
   colordebugger
   pspuser
   pspsystemctrl_user
   pspkubridge
   psppower
   vlfgui
   vlfgu
   vlfutils
   vlflibc
   pspdecrypt
   psppsar
   pspdebug
   pspdisplay
   pspge
   pspctrl
   pspnet
   pspnet_apctl
   pspmodinfo
)

create_pbp_file(
   TARGET arkdcinstaller
   TITLE "ARK DC Installer"
   ICON_PATH ${CMAKE_CURRENT_SOURCE_DIR}/res/cementerio.png
   BACKGROUND_PATH ${CMAKE_CURRENT_SOURCE_DIR}/res/despertar.png
   BUILD_PRX
)

get_target_property(BTCNF_FILES build_btcnfs BTCNF_FILES)

foreach(BTCNF IN LISTS BTCNF_FILES)
   
   get_filename_component(BTCNF_NAME "${BTCNF}" NAME_WE)

   add_custom_command(
      OUTPUT ${BTCNF_NAME}.h
      DEPENDS build_btcnfs ${BTCNF}
      COMMAND bin2c "${BTCNF}" ${BTCNF_NAME}.h ${BTCNF_NAME}
   )
endforeach()
