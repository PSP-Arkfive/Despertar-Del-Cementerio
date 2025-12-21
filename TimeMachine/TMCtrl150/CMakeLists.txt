add_prx_module(tmctrl150 exports.exp)

remove_definitions("-D_PSP_FW_VERSION=600")
add_definitions("-D_PSP_FW_VERSION=150")

target_sources(tmctrl150 PRIVATE main.c flashemu.c)
target_link_libraries(tmctrl150 PRIVATE ${TMCTRL_MOD_LIBS})