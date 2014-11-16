#version 330

in VertexData {
	vec3 normal;
	float height;
    vec3 viewDir;
    float depth;
} FragIn;

out vec4 out_Color;

const vec3 lightDir = normalize(vec3(1.f, -1.f, 0.6f));
const vec4 lightColor = vec4(0.,1,0.,1);
const vec4 ambientLightColor = vec4(0.3,0.3,0.,1); // gray

uniform float waterLevel;

void main(void)
{
	float dotprod = (dot(lightDir, FragIn.normal));
	dotprod = max(dotprod, 0.0);
	out_Color = 
	0.3*ambientLightColor 
	+ lightColor*dotprod
	+ (FragIn.height<waterLevel?vec4(0,0,1.,1):vec4(0));//*dotprod;

	out_Color.a = 1.0f;
	//out_Color.rgb = vec3(FragIn.depth);
	//out_Color.rgb = FragIn.normal;
}