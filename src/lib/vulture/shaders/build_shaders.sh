#BASE
glslangValidator -V wire.vert -o wire.vert.spv
#glslangValidator -V wire.geom -o wire.geom.spv
glslangValidator -V wire.frag -o wire.frag.spv

# AXIS
glslangValidator -V wire.grid.geom -o wire.grid.geom.spv

# AXIS
# Wireframe Shaders
glslangValidator -V wire.axis.vert -o wire.axis.vert.spv
glslangValidator -V wire.axis.geom -o wire.axis.geom.spv
glslangValidator -V wire.axis.frag -o wire.axis.frag.spv

# MESH
# Wireframe Shaders
glslangValidator -V wire.mesh.vert -o wire.mesh.vert.spv
glslangValidator -V wire.mesh.geom -o wire.mesh.geom.spv
glslangValidator -V wire.mesh.frag -o wire.mesh.frag.spv
# Point Shaders
glslangValidator -V point.mesh.vert -o point.mesh.vert.spv
glslangValidator -V point.mesh.geom -o point.mesh.geom.spv
glslangValidator -V point.mesh.frag -o point.mesh.frag.spv
# Base Shaders
glslangValidator -V shade.mesh.vert -o shade.mesh.vert.spv
glslangValidator -V shade.mesh.geom -o shade.mesh.geom.spv
glslangValidator -V shade.mesh.frag -o shade.mesh.frag.spv
# Select Shaders
glslangValidator -V select.mesh.vert -o select.mesh.vert.spv
glslangValidator -V select.mesh.frag -o select.mesh.frag.spv

# LIGHTS 
# Wireframe Shaders
glslangValidator -V wire.light.vert -o wire.light.vert.spv
glslangValidator -V wire.light.geom -o wire.light.geom.spv
glslangValidator -V wire.light.frag -o wire.light.frag.spv
# Point Shaders
#glslangValidator -V point.light.vert -o point.light.vert.spv
#glslangValidator -V point.light.geom -o point.light.geom.spv
#glslangValidator -V point.light.frag -o point.light.frag.spv
# Base Shaders
glslangValidator -V shade.light.vert -o shade.light.vert.spv
glslangValidator -V shade.light.frag -o shade.light.frag.spv


