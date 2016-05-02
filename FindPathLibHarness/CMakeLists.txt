
add_definitions(
    -DFINDPATHLIB_EXPORTS
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

set(FindPathLibHarness_SRCS
    FindPathLibHarness.cpp
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/dep/detour
  ${CMAKE_SOURCE_DIR}/FindPathLib
  ${Boost_INCLUDE_DIR}
)

add_executable(FindPathLibHarness
    ${FindPathLibHarness_SRCS}
)

target_link_libraries(
    FindPathLibHarness
    FindPathLib
    detour
    ${Boost_LIBRARIES}
)
