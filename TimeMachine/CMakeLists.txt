set(TMCTRL_MOD_LIBS -nostdlib pspsystemctrl_kernel pspmodinfo pspsdk pspkernel)
add_compile_options(-std=c99 -Os -G0 -Wall -fno-pic -fshort-wchar)

add_subdirectory(IDStorage)
add_subdirectory(rebootex)

add_prx_module(tmctrl660 exports.exp)

target_sources(tmctrl660 PRIVATE main.c flashemu.c payload.h cptbl.h)
target_link_libraries(tmctrl660 PRIVATE cisoread ${TMCTRL_MOD_LIBS} pspsemaphore_660)
target_include_directories(tmctrl660 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(OUTPUT payload.h
  DEPENDS tmctrl660_reboot
  COMMAND bin2c "$<TARGET_FILE:tmctrl660_reboot>.bin.gz" payload.h rebootbuffer_ms_psp
)

add_custom_command(OUTPUT cptbl.h
  COMMAND dd if=${PROJECT_SOURCE_DIR}/Resources/cptbl.dat of=cptbl.dat bs=2048 conv=sync
	COMMAND python3 "$<TARGET_PROPERTY:tool_ciso,EXEC>" -c 1 cptbl.dat cptbl.cso
	COMMAND bin2c cptbl.cso cptbl.h cptbl
)
