# -DCMAKE_HOST_SYSTEM_PROCESSOR=x86_64 -DCMAKE_SYSTEM_PROCESSOR=x86_64 set(CMAKE_OSX_ARCHITECTURES "x86_64")
cmake_minimum_required(VERSION 3.18)
project(mao)

set(CMAKE_CXX_STANDARD 23)

if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE Release)
endif()

# 设置调试版本后缀
set(CMAKE_DEBUG_POSTFIX "_d")

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
    message("++++ current platform: Linux ")
    add_definitions(-DLINUX)
elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    message("++++ current platform: Windows ")
    add_definitions(-DWINDOWS)
elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin")
    message("++++ current platform: Darwin ")
    add_definitions(-DDARWIN)
    set(CMAKE_OSX_ARCHITECTURES "x86_64")
    set(CMAKE_HOST_SYSTEM_PROCESSOR "x86_64")
    set(CMAKE_SYSTEM_PROCESSOR "x86_64")
else()
    message(FATAL_ERROR "++++ current platform: unkown ")
endif()

# 设置路径
set(THIRD_PARTY ${CMAKE_SOURCE_DIR}/3rd_party)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/test)
set(OUT_DIR ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

# 设置输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUT_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUT_DIR}/lib)
set(CMAKE_INSTALL_BINDIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_INSTALL_LIBDIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

message("++++ Set Runtime Out Dir: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message("++++ Set Library Out Dir: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message("++++ Set Archive Out Dir: ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
message("++++ Set Bin Install Out Dir: ${CMAKE_INSTALL_BINDIR}")
message("++++ Set Lib Install Out Dir: ${CMAKE_INSTALL_LIBDIR}")

add_subdirectory(src)
add_subdirectory(test)

