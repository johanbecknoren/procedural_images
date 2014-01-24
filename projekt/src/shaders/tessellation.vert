#version 410 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_texCoord;

uniform mat4 MVP;

out VertexData {
	vec3 world_pos_tcs;
	vec2 tex_coord_tcs;
	vec3 normal_tcs;
} VertexOut;

void main(void) {
	VertexOut.world_pos_tcs = (MVP * vec4(in_Position, 1.0f)).xyz;
	VertexOut.tex_coord_tcs = in_texCoord;
	VertexOut.normal_tcs 	= (MVP * vec4(in_Normal, 1.0f)).xyz;
}