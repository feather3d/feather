#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (lines) in;
layout (line_strip, max_vertices = 44) out;

layout (binding = 1) uniform UBO 
{
	mat4 projection;
	mat4 model;
        int mode;
} ubo;

layout (location = 0) in vec3 inNormal[]; 
layout (location = 1) in vec3 inId[]; 
layout (location = 2) in vec3 inColor[];

layout (location = 0) out vec3 outColor;

void main() 
{
    vec3 p0 = gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_in[1].gl_Position.xyz;

    int i=0;
    float xstep=0;
    float ystep=0;
    float zstep=0;
    float c0x=p0.x;
    float c0y=p0.y;
    float c0z=p0.z;
    float c1x=p1.x;
    float c1y=p1.y;
    float c1z=p1.z;

    if(p0.x == p1.x)
        xstep = abs(p0.x)/10.0;
    if(p0.y == p1.y)
        ystep = abs(p0.y)/10.0;
    if(p0.z == p1.z)
        zstep = abs(p0.z)/10.0;

    while(i <= 20) {
        gl_Position = ubo.projection * (ubo.model * vec4(c0x, c0y, c0z, 1.0));
        outColor = inColor[0];
        EmitVertex();

        gl_Position = ubo.projection * (ubo.model * vec4(c1x, c1y, c1z, 1.0));
        outColor = inColor[1];
        EmitVertex();

        EndPrimitive();

        c0x=c0x - xstep;
        c0y=c0y - ystep;
        c0z=c0z + zstep;
        c1x=c1x - xstep;
        c1y=c1y - ystep;
        c1z=c1z + zstep;

        i++;
    }

}
