cmake_minimum_required(VERSION 3.28)

project(dcark VERSION 1.0 LANGUAGES C ASM)
set(PSPCFWSDK_PATH "" CACHE PATH "Path to local psp-cfw-sdk (for local development)")
set(DEBUG "" CACHE STRING "Debug level (1,2,3)")
message(STATUS "DEBUG LEVEL = ${DEBUG}")

set(DIST_DIR ${CMAKE_CURRENT_LIST_DIR}/dist)

include(FetchContent)
if(PSPCFWSDK_PATH)
message(STATUS "Using local pspcfw repo: ${PSPCFWSDK_PATH}")
FetchContent_Declare(
   pspcfwsdk
   SOURCE_DIR ${PSPCFWSDK_PATH}
   EXCLUDE_FROM_ALL
)
else()
FetchContent_Declare(
   pspcfwsdk
   GIT_REPOSITORY https://github.com/pspdev/psp-cfw-sdk.git
   GIT_TAG main
   EXCLUDE_FROM_ALL
)
endif()
FetchContent_MakeAvailable(pspcfwsdk)

set(CFWSDK ${pspcfwsdk_SOURCE_DIR})

remove_definitions("-D_PSP_FW_VERSION=600")
add_definitions("-D_PSP_FW_VERSION=660")

add_subdirectory(btcnf)
add_subdirectory(DCManager)
add_subdirectory(Installer)
add_subdirectory(TimeMachine)
add_subdirectory(VUnbricker)

add_custom_target(dist
   DEPENDS
      dcman
      arkdcinstaller
      resurrection
   COMMAND ${CMAKE_COMMAND} -E make_directory ${DIST_DIR}/DC10
   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:dcman>.prx ${DIST_DIR}/DC10
   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:resurrection>.prx ${DIST_DIR}/DC10
   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:arkdcinstaller>/EBOOT.PBP ${DIST_DIR}/DC10
   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:tmctrl150>.prx ${DIST_DIR}/DC10
   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:tmctrl660>.prx ${DIST_DIR}/DC10
)
