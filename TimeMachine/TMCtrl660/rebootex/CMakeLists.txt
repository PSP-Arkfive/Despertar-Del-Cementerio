add_executable(tmctrl660_reboot)
target_sources(tmctrl660_reboot PRIVATE main.c)
target_link_libraries(tmctrl660_reboot PRIVATE bootloadex)

if(DEBUG)
    target_compile_definitions(tmctrl660_reboot PRIVATE -DDEBUG=${DEBUG})
    target_link_libraries(tmctrl660_reboot PRIVATE colordebugger)
endif()

add_custom_command(
    TARGET tmctrl660_reboot
    POST_BUILD
    COMMAND psp-strip -s tmctrl660_reboot
    COMMAND psp-objcopy -O binary tmctrl660_reboot tmctrl660_reboot.bin
    COMMAND python3 $<TARGET_PROPERTY:tool_gz,EXEC> tmctrl660_reboot.bin tmctrl660_reboot.bin.gz
)
