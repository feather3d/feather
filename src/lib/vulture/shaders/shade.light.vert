#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;
layout (location = 3) in vec3 inColor;

layout (binding = 0) uniform UBO 
{
	mat4 projection;
	mat4 model;
        int mode;
} ubo;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec2 outUV;
layout (location = 2) out vec3 outColor;
layout (location = 3) out vec3 outEyePos;
layout (location = 4) out vec3 outLightVec;

void main() 
{
    outUV = inUV.st;
    outNormal = inNormal;
    outColor = inColor;
    outEyePos = vec3(ubo.model * vec4(inPosition,1.0));
    vec4 lightPos = vec4(0.0, 0.0, 0.0, 1.0) * ubo.model;
    outLightVec = normalize(lightPos.xyz - outEyePos);
    gl_Position = ubo.projection * ubo.model * vec4(inPosition.xyz, 1.0);
}
