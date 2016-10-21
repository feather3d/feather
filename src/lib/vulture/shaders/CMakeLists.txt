PROJECT(feather_spv_shaders)
FILE(GLOB SPIRV_FILES "*.spv")
INSTALL(FILES ${SPIRV_FILES} DESTINATION /usr/local/feather/shaders/spv)

# this is a temp fix for vulture shaders
INSTALL(FILES ${SPIRV_FILES} DESTINATION /usr/local/vulture/shaders/spv)
