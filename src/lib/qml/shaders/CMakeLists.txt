PROJECT(feather_shaders)
FILE(GLOB SPIRV_FILES "*.glsl")
INSTALL(FILES ${SPIRV_FILES} DESTINATION /usr/share/feather/ui/shaders)
