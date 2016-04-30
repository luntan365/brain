cmake_minimum_required(VERSION 3.3)

project(FancyBot)


set(BIN_DIR ${CMAKE_INSTALL_PREFIX}/)
set(LIBS_DIR ${CMAKE_INSTALL_PREFIX}/) 

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(BOOST_ROOT C:/boost_1_60_0)
set(BOOST_LIBRARYDIR ${BOOST_ROOT}/stage/lib/)

set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(BOOST_ALL_DYN_LINK OFF)

find_package(Boost 1.60.0 COMPONENTS 
    context
    coroutine
    date_time
    filesystem
    regex
    system
)

find_library(SHLWAPI Shlwapi.lib)
find_library(UDIS libudis86.lib ${CMAKE_SOURCE_DIR}/dep/udis86/BuildVS2010/Build/Lib/x86)

set(ASMJIT_STATIC TRUE)
set(ASMJIT_BUILD_X86 TRUE)
set(ASMJIT_BUILD_X64 TRUE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Z7")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /Z7 /SAFESEH:NO")

add_subdirectory(dep)

add_subdirectory(FancyBotBrain)
add_subdirectory(FancyBotInjector)

