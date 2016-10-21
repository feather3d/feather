#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (binding = 1) uniform sampler2D samplerColorMap;

layout (location = 0) in vec3 inNormal;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inColor;
layout (location = 3) in vec3 inEyePos;
layout (location = 4) in vec3 inLightVec;
layout (location = 5) in flat ivec4 inId;

layout (location = 0) out vec4 outColor;
layout (location = 1) out ivec4 outSelection;

void main() 
{
    outColor = vec4(1,0,0,1);
    outSelection= inId;
}
