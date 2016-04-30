
add_definitions(
    -DFANCYBOTBRAIN_EXPORTS
    -DWIN32
    -D_WINDOWS
    -D_USRDLL
    -DSTRICT
    -DSTRICT_TYPED_ITEMIDS
    -DUNICODE
    -D_UNICODE
    -D_CRT_SECURE_NO_WARNINGS
    -D_SCL_SECURE_NO_WARNINGS
    -DASMJIT_BUILD_X86
    -DASMJIT_BUILD_X64
    -DASMJIT_STATIC
    -DLIBUDIS86_STATIC
    -DCROW_MSVC_WORKAROUND
    -DDT_POLYREF64
    -DWINVER=0x0601
    -D_WIN32_WINNT=0x0601
)

set(Brain_SRCS
    dllmain.cpp
    EndSceneManager.cpp
    FancyBotBrain.cpp
    GameState.cpp
    HttpApi.cpp
    WoWObject.cpp
    WoWObjectManager.cpp
    WowOffsets.cpp
    WoWPlayer.cpp
    WoWUnit.cpp
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/dep/detour
  ${CMAKE_SOURCE_DIR}/dep/asmjit/src
  ${CMAKE_SOURCE_DIR}/dep/udis86
  ${CMAKE_SOURCE_DIR}/dep/udis86/libudis86
  ${CMAKE_SOURCE_DIR}/dep/hadesmem/include/memory
  ${Boost_INCLUDE_DIR}
)

add_library(FancyBotBrain SHARED
  ${Brain_SRCS}
)

target_link_libraries(
    FancyBotBrain
    asmjit
    ${Boost_LIBRARIES}
    ${SHLWAPI}
    ${UDIS}
)
