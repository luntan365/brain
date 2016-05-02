
add_definitions(
    -DWIN32
    -D_WINDOWS
    -D_USRDLL
    -DSTRICT
    -DSTRICT_TYPED_ITEMIDS
    -DUNICODE
    -D_UNICODE
    -D_CRT_SECURE_NO_WARNINGS
    -D_SCL_SECURE_NO_WARNINGS
    -DCROW_MSVC_WORKAROUND
    -DDT_POLYREF64
    -DWINVER=0x0601
    -D_WIN32_WINNT=0x0601
)

set(FindPathLib_SRCS
    MoveMap.cpp
    MoveMapManager.cpp
    MoveMapTile.cpp
    PathFinder.cpp
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

add_library(FindPathLib STATIC
    ${FindPathLib_SRCS}
)

target_link_libraries(
    FindPathLib
    detour
    ${Boost_LIBRARIES}
)
