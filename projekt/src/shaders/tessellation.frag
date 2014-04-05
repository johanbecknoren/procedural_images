#version 410 core

in TESOut {
	vec3 world_pos_frag;
	vec2 tex_coord_frag;
	vec3 normal_frag;
} TESData;

out vec4 outColor;

void main(void)
{
	//outColor = vec4(TESData.world_pos_frag.xyz, 1.0f);
	//outColor = vec4(1.0f,0.0f,0.0f,1.0f);
	//outColor = vec4(TESData.tex_coord_frag.sst, 1.0f);
	outColor = vec4(TESData.normal_frag, 1.0f);
}
