#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
//#extension GL_EXT_gpu_shader4: enable

layout (location = 0) in vec3 inPosition;
layout (location = 3) in vec3 inColor;
layout (location = 4) in ivec4 inId;

layout (binding = 0) uniform UBO 
{
	mat4 projection;
	mat4 model;
        int mode;
        // current ids under the mouse
        int point;
        int edge;
        int face;
        int object;
} ubo;

layout (location = 0) out vec3 outColor;
layout (location = 1) out ivec4 outId;
layout (location = 2) out int outPoint;

void main() 
{
    outColor = inColor;
    gl_Position = vec4(inPosition.xyz, 1.0);
    outId = inId;
    outPoint = ubo.point;
}
