
add_definitions(
    -DWIN32
    -D_CONSOLE
    -D_LIB
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
    -DDT_POLYREF64
)

set(Injector_SRCS
    FancyBotInjector.cpp
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

add_executable(injector 
    ${Injector_SRCS}
)

target_link_libraries(
    injector
    asmjit
    ${Boost_LIBRARIES}
    ${SHLWAPI}
)
