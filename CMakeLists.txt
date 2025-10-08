cmake_minimum_required(VERSION 3.23)

project(monad_event_sdk_macos_compat VERSION 1.0 LANGUAGES C)

add_library(monad_event_macos_compat
  "lib/macos/posix_fallocate.c")

target_sources(monad_event_macos_compat PUBLIC
  FILE_SET compat_headers
  TYPE HEADERS
  BASE_DIRS "include"
  FILES
  "include/fcntl.h"
  "include/monad_compat/common/stdbit.h"
  "include/string.h")

target_include_directories(monad_event_macos_compat PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
