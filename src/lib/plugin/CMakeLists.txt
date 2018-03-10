project(feather_plugin)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(feather_plugin_SRCS
    core_access.cpp
    plugin.cpp
)

SET_SOURCE_FILES_PROPERTIES(${feather_plugin_SRCS}
 PROPERTIES
 COMPILE_FLAGS ${CMAKE_CXX_FLAGS} )

INCLUDE_DIRECTORIES(${feather_core_SOURCE_DIR})

LINK_DIRECTORIES(${feather_core_BINARY_DIR})

LINK_LIBRARIES(${feather_core})

ADD_LIBRARY(feather_plugin SHARED ${feather_plugin_SRCS})

SET_TARGET_PROPERTIES(feather_plugin
    PROPERTIES
    CMAKE_CXX_STANDARD 14
    CMAKE_CXX_STANDARD_REQUIRED ON
)

SET_TARGET_PROPERTIES(feather_plugin
    PROPERTIES
    VERSION 0.01
    SOVERSION 1
)

SET(feather_plugin_HDRS
    plugin.hpp
)

INSTALL(FILES ${feather_plugin_HDRS}
    DESTINATION /usr/include/feather)

INSTALL(TARGETS feather_plugin
    LIBRARY DESTINATION /usr/lib/feather)
