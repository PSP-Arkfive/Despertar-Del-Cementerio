file(GLOB BTCNF_TXT_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/pspbtcnf*.txt")

foreach(BTCNF_TXT_FILE IN LISTS BTCNF_TXT_FILES)
   get_filename_component(BTCNF_NAME "${BTCNF_TXT_FILE}" NAME_WE)
   add_custom_command(
      OUTPUT ${BTCNF_NAME}.bin
      DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${BTCNF_NAME}.txt"
      COMMAND python3 $<TARGET_PROPERTY:tool_btcnf,EXEC> build "${CMAKE_CURRENT_SOURCE_DIR}/${BTCNF_NAME}.txt"
      COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${BTCNF_NAME}.bin" ${CMAKE_CURRENT_BINARY_DIR}/
   )

   list(APPEND BTCNF_BIN_FILES "${BTCNF_NAME}.bin")
   list(APPEND BTCNF_BIN_PATHS "${CMAKE_CURRENT_BINARY_DIR}/${BTCNF_NAME}.bin")
endforeach()

add_custom_target(build_btcnfs DEPENDS ${BTCNF_BIN_FILES})
set_target_properties(build_btcnfs PROPERTIES BTCNF_FILES "${BTCNF_BIN_PATHS}")