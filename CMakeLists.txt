cmake_minimum_required(VERSION 3.5)

if (APPLE AND UNIX)
  SET(CMAKE_OSX_SYSROOT /var/theos/sdks/iPhoneOS11.2.sdk)
endif(APPLE AND UNIX)

project(stegano)

SET(CMAKE_C_FLAGS "-Wall -Wno-unused-command-line-argument")

file(GLOB SOURCES "src/*.c" "src/*/*.c" "src/formats/*/*.c")

add_executable(stegano ${SOURCES})
target_link_libraries(stegano z -static)
