#version 330

in VertexData {
	vec3 pos;
	vec3 normal;
    vec3 viewDir;
    float height;
    float depth;
} FragIn;

out vec4 out_Color;

const vec3 lightDir = normalize(vec3(1.f, 0.5f,0.f));
const vec4 lightColor = vec4(0.,1,0.,1);
const vec4 ambientLightColor = vec4(0.3,0.3,0.3,1); // gray

void main(void)
{
	//if(dot(FragIn.viewDir, FragIn.normal)>0)
	//	discard;
	float dotprod = (dot(lightDir, FragIn.normal));
	dotprod = dotprod>0?dotprod:0.0f;
	out_Color = 
	0.2*ambientLightColor 
	+ lightColor*dotprod
	//+ (FragIn.height>0.5f?vec4(1,1,1,1):vec4(0))
	+ (FragIn.height<0.5f?vec4(0,0,1.,1):vec4(0));//*dotprod;

	out_Color.a = 1.0f;
	//out_Color.rgb = vec3(FragIn.depth);
}