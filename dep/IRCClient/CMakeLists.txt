
set(ircclient_SRCS
    src/IRCClient.cpp
    src/IRCHandler.cpp
    src/IRCSocket.cpp
    src/Thread.cpp
)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/dep/IRCClient/src
)

add_library(ircclient STATIC
  ${ircclient_SRCS}
)
