#version 450

layout (std140, binding = 0) uniform bufferVals {	// DESCRIPTOR_SET_BINDING_INDEX
    mat4 mvp;
} myBufferVals;

layout (location = 0) in vec4 pos;
layout (location = 1) in vec2 inUV;
layout (location = 0) out vec2 outUV;

void main()
{
    outUV 		 = inUV;
    gl_Position 	 = myBufferVals.mvp * pos;
    gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;
}