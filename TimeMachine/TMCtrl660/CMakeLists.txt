add_subdirectory(rebootex)

add_prx_module(tmctrl660 exports.exp)

remove_definitions("-D_PSP_FW_VERSION=600")
add_definitions("-D_PSP_FW_VERSION=660")

target_sources(tmctrl660 PRIVATE main.c flashemu.c payload.h)
target_link_libraries(tmctrl660 PRIVATE ${TMCTRL_MOD_LIBS} pspsemaphore_660)
target_include_directories(tmctrl660 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(OUTPUT payload.h
  DEPENDS tmctrl660_reboot
  COMMAND bin2c "$<TARGET_FILE:tmctrl660_reboot>.bin.gz" payload.h rebootbuffer_ms_psp
)
