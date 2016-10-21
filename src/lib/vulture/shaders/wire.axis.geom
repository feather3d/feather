#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (lines) in;
layout (line_strip, max_vertices = 2) out;

layout (binding = 1) uniform UBO 
{
	mat4 projection;
	mat4 model;
        int mode;
} ubo;

layout (location = 0) in vec3 inNormal[]; 
layout (location = 1) in uint inSelection[]; 
layout (location = 2) in vec3 inColor[];

layout (location = 0) out vec3 outColor;

void main() 
{
    vec3 p0 = vec3(0,0,0);
    vec3 p1 = gl_in[1].gl_Position.xyz;

    gl_Position = ubo.projection * (ubo.model * vec4(p0, 1.0));
    outColor = inColor[1];
    EmitVertex();

    gl_Position = ubo.projection * (ubo.model * vec4(p1, 1.0));
    outColor = inColor[1];
    EmitVertex();

    EndPrimitive();
}
