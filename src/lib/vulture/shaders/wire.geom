#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

layout (binding = 1) uniform UBO 
{
	mat4 projection;
	mat4 model;
        int mode;
} ubo;

layout (location = 0) in vec3 inNormal[]; 
layout (location = 1) in vec4 inId[]; 

layout (location = 0) out vec3 outColor;
layout (location = 1) out vec4 outId;

void main() 
{
    float normalLength = 0.15;
    float pointSize = 4.0;

    for(int i=0; i<gl_in.length(); i++)
    {
        vec3 pos = gl_in[i].gl_Position.xyz;
        vec3 normal = inNormal[i].xyz;

        gl_Position = ubo.projection * (ubo.model * vec4(pos, 1.0));
        gl_PointSize = pointSize;
        outColor = vec3(1.0, 0.0, 0.0);
        outId = inId[i]
        EmitVertex();

        gl_Position = ubo.projection * (ubo.model * vec4(pos + normal * normalLength, 1.0));
        gl_PointSize = pointSize;
        outColor = vec3(0.0, 0.0, 1.0);
        outId = inId[i]
        EmitVertex();

        EndPrimitive();
    }
  
    int val = 1;
}
