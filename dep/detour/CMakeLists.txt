
add_definitions(-DDT_POLYREF64)

set(detour_SRCS
    DetourAlloc.cpp
    DetourCommon.cpp
    DetourNavMeshBuilder.cpp
    DetourNavMesh.cpp
    DetourNavMeshQuery.cpp
    DetourNode.cpp
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/dep/detour
)

add_library(detour STATIC
  ${detour_SRCS}
)
