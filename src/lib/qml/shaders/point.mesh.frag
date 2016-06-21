#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 inColor;
layout (location = 1) in flat ivec4 inId;
layout (location = 2) in flat int inPoint;

layout (location = 0) out vec4 outColor;
layout (location = 1) out ivec4 outSelection;

void main() 
{
    if(inId.r == inPoint && inId.g == 0 && inId.b == 0)
        outColor = vec4(1,0,1,1);
    else
        outColor = vec4(0,0,1,1);
    outSelection = inId;
}
