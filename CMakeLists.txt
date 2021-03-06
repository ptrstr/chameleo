cmake_minimum_required(VERSION 3.5)

EXECUTE_PROCESS( COMMAND uname -p COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )

if (APPLE AND UNIX AND ${ARCHITECTURE} STREQUAL "arm64")
  SET(CMAKE_OSX_SYSROOT /var/theos/sdks/iPhoneOS11.2.sdk)
endif (APPLE AND UNIX AND ${ARCHITECTURE} STREQUAL "arm64")

project(chameleo)

if (UNIX)
  SET(CMAKE_C_FLAGS "-Wall -Wno-unused-command-line-argument")
endif (UNIX)

file(GLOB SOURCES "src/*.c" "src/*/*.c" "src/formats/*/*.c")

add_executable(chameleo ${SOURCES})
if (NOT APPLE AND UNIX)
  target_link_libraries(chameleo z -static)
endif (NOT APPLE AND UNIX)

if (APPLE)
  target_link_libraries(chameleo z)
endif (APPLE)
