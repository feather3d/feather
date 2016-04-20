#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

layout (binding = 1) uniform UBO 
{
	mat4 projection;
	mat4 model;
        int mode;
        // selection ids
        int p1;
        int p2;
        int p3;
        int object;
} ubo;

layout (location = 0) in vec3 inNormal[]; 
layout (location = 1) in vec2 inUV[];
layout (location = 2) in vec3 inColor[];
layout (location = 3) in vec3 inEyePos[];
layout (location = 4) in vec3 inLightVec[];
layout (location = 5) in ivec4 inId[];

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec2 outUV;
layout (location = 2) out vec3 outColor;
layout (location = 3) out vec3 outEyePos;
layout (location = 4) out vec3 outLightVec;
layout (location = 5) out ivec4 outId;

void main() 
{

    vec4 p0 = gl_in[0].gl_Position;
    vec4 p1 = gl_in[1].gl_Position;
    vec4 p2 = gl_in[2].gl_Position;

    // P0
    gl_PrimitiveID = gl_PrimitiveIDIn;
    gl_Position = p0;
 
    outNormal = inNormal[0];
    outUV = inUV[0];
    outEyePos = inEyePos[0];
    outLightVec = inLightVec[0];
    outColor = inColor[0];
    outId.r = inId[0].r;
    outId.g = inId[1].r;
    outId.b = inId[2].r;
    outId.a = inId[0].a;
    EmitVertex();

    // P1
    gl_PrimitiveID = gl_PrimitiveIDIn;
    gl_Position = p1;
  
    outNormal = inNormal[1];
    outUV = inUV[1];
    outEyePos = inEyePos[1];
    outLightVec = inLightVec[1];
    outColor = inColor[1];
    outId.r = inId[0].r;
    outId.g = inId[1].r;
    outId.b = inId[2].r;
    outId.a = inId[1].a;
    EmitVertex();

    // P2
    gl_PrimitiveID = gl_PrimitiveIDIn;
    gl_Position = p2;
  
    outNormal = inNormal[2];
    outUV = inUV[2];
    outEyePos = inEyePos[2];
    outLightVec = inLightVec[2];
    outColor = inColor[2];
    outId.r = inId[0].r;
    outId.g = inId[1].r;
    outId.b = inId[2].r;
    outId.a = inId[2].a;
    EmitVertex();

    EndPrimitive();
}
