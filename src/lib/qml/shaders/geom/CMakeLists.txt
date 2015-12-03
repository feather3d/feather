project(feather_geom_shaders)
FILE(GLOB GLSL_FILES "*.glsl")
INSTALL(FILES ${GLSL_FILES} DESTINATION /usr/local/feather/shaders/geom)
