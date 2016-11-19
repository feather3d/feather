PROJECT(feather_vulture)

#SET(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE(PythonLibs REQUIRED)
FIND_PACKAGE(Boost COMPONENTS filesystem system REQUIRED)
#FIND_PACKAGE(Qt5Widgets REQUIRED)
#FIND_PACKAGE(Qt5Gui REQUIRED)
#FIND_PACKAGE(Qt5Core REQUIRED)

SET(CMAKE_CXX_LINK_FLAGS "-lvulkan")
#SET(CMAKE_CXX_LINK_FLAGS "-lassimp")
SET(CMAKE_CXX_LINK_FLAGS "-lxcb")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_XCB_KHR")

MESSAGE(STATUS ${VULKAN_LIB})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX -DVK_PROTOTYPES -D_USE_MATH_DEFINES")

ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
#ADD_DEFINITIONS(-std=c++11)
#ADD_DEFINITIONS(-std=c++0x)

SET(feather_vulture_SRCS
    tools.cpp
    debug.cpp
    basenode.cpp
    axis.cpp
    grid.cpp
    mesh.cpp
    pointlight.cpp
    pipelines.cpp
    window.cpp
    #qvulkanwindow.cpp
)

SET(pyvulture_SRCS
    vulture_wrap.cxx
)

SET(vulture_cpp_SRCS
    vulture_cpp.cpp
)


SET_SOURCE_FILES_PROPERTIES(${feather_vulture_SRCS}
 PROPERTIES
 COMPILE_FLAGS ${CMAKE_CXX_FLAGS}
)

SET_SOURCE_FILES_PROPERTIES(${pyvulture_SRCS}
 PROPERTIES
 COMPILE_FLAGS ${CMAKE_CXX_FLAGS}
)

INCLUDE_DIRECTORIES(${feather_core_SOURCE_DIR}
 ${feather_plugin_SOURCE_DIR}
 ${PYTHON_INCLUDE_DIRS}
 #${Qt5Gui_INCLUDE_DIRS}
 #${Qt5Core_INCLUDE_DIRS}
 #${Qt5Widgets_INCLUDE_DIRS}
)
 
LINK_DIRECTORIES(${feather_core_BINARY_DIR}
    ${feather_plugin_BINARY_DIR}
)

SET(pyvulture_LIBS
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    #${Qt5Widgets_LIBRARIES}
    #${Qt5Quick_LIBRARIES}
    #${Qt5Core_LIBRARIES}
)

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include/boost")


LINK_LIBRARIES(feather_core feather_plugin)

ADD_LIBRARY(feather_vulture SHARED ${feather_vulture_SRCS})
TARGET_LINK_LIBRARIES(feather_vulture "-lvulkan" "-lboost_system" ${PTHREAD} ${pyvulture_LIBS})
#TARGET_LINK_LIBRARIES(pyvulture "-lvulkan" ${PTHREAD} ${PYTHON_LIBRARIES} feather_vulture)


SET_TARGET_PROPERTIES(feather_vulture
    PROPERTIES
    CMAKE_CXX_STANDARD 14
    CMAKE_CXX_STANDARD_REQUIRED ON
)

SET_TARGET_PROPERTIES(feather_vulture
    PROPERTIES
    VERSION 0.01
    SOVERSION 1
)

UNSET(CMAKE_SHARED_LIBRARY_PREFIX)
ADD_LIBRARY(_pyvulture SHARED ${pyvulture_SRCS})
#TARGET_LINK_LIBRARIES(_pyvulture "-lxcb" ${PYTHON_LIBRARIES} feather_vulture)
TARGET_LINK_LIBRARIES(_pyvulture feather_vulture)

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include/boost")

SET_TARGET_PROPERTIES(_pyvulture
    PROPERTIES
    CMAKE_CXX_STANDARD 14
    CMAKE_CXX_STANDARD_REQUIRED ON
)

SET_TARGET_PROPERTIES(_pyvulture
    PROPERTIES
    VERSION 0.01
    SOVERSION 1
)

# EXECUTABLE TEST

ADD_EXECUTABLE(vulture_cpp ${vulture_cpp_SRCS})

SET_TARGET_PROPERTIES(vulture_cpp
    PROPERTIES
    CMAKE_CXX_STANDARD 11
    CMAKE_CXX_STANDARD_REQUIRED ON
)

TARGET_LINK_LIBRARIES(vulture_cpp ${pyvulture_LIBS} feather_vulture ${CMAKE_DL_LIBS})

# INSTALL
INSTALL(TARGETS feather_vulture
    LIBRARY DESTINATION /usr/lib64/feather)

# HEADERS
SET(feather_vulture_HDRS
    window.hpp
)

INSTALL(FILES ${feather_vulture_HDRS}
    DESTINATION /usr/include/feather/vulkan)

# SHADERS
add_subdirectory(shaders)
