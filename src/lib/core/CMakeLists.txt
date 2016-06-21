project(feather_core)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE(Boost COMPONENTS filesystem system REQUIRED)
FIND_PACKAGE(Qt5OpenGL REQUIRED)

SET(QT_USE_QTOPENGL TRUE)

SET(feather_core_SRCS
    pluginmanager.cpp
)

SET_SOURCE_FILES_PROPERTIES(${feather_core_SRCS}
 PROPERTIES
 COMPILE_FLAGS ${CMAKE_CXX_FLAGS} )

INCLUDE_DIRECTORIES(${BOOST_INCLUDE_DIRS}
    ${QtOpenGL_INCLUDE_DIRS}
)

SET(feather_core_LIBS
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
)

ADD_LIBRARY(feather_core SHARED ${feather_core_SRCS})

QT5_USE_MODULES(feather_core OpenGL)

TARGET_LINK_LIBRARIES(feather_core ${feather_core_LIBS}  ${CMAKE_DL_LIBS})

#SET(CMAKE_CXX_FLAGS "-I/usr/include/boost")

SET_TARGET_PROPERTIES(feather_core
    PROPERTIES
    CMAKE_CXX_STANDARD 14
    CMAKE_CXX_STANDARD_REQUIRED ON
)

SET_TARGET_PROPERTIES(feather_core
    PROPERTIES
    VERSION 0.01
    SOVERSION 1
)

SET(feather_core_HDRS
    deps.hpp
    types.hpp
    singleton.hpp
    scenegraph.hpp
    pluginmanager.hpp
    types.hpp
    state.hpp
    status.hpp
    parameter.hpp
    command.hpp
    field.hpp
    node.hpp
    data.hpp
    selection.hpp
    object.hpp
    camera.hpp
    light.hpp
    texture.hpp
    shader.hpp
    draw.hpp
)

INSTALL(FILES ${feather_core_HDRS}
    DESTINATION /usr/include/feather)

INSTALL(TARGETS feather_core
    LIBRARY DESTINATION /usr/lib64/feather)
