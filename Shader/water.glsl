#version 330

#include "global_uniform.glsl"


#ifdef _VERTEX_
in vec3 in_Vertex;
in vec2 in_TexCoord0;
out vec2 texCoord0;




void main()
{
    gl_Position = u_matProjection*u_matView*u_matModel*vec4(in_Vertex,1.0);
	texCoord0 = in_TexCoord0;
}
#endif

#ifdef _FRAGMENT_

uniform int u_textureFlags;
uniform sampler2D texture5, texture2;
uniform samplerCube texture0;
uniform vec3 u_dir;
in vec2 texCoord0;

out vec4 out_Color;

const float Eta = 0.15; // Water

const vec3 lightDir = vec3(1.0f,1.0f,-1.0f);

vec3 textureToNormal(vec4 orgNormalColor)
{
	return normalize(vec3(clamp(orgNormalColor.r*2.0 - 1.0, -1.0, 1.0), clamp(orgNormalColor.g*2.0 - 1.0, -1.0, 1.0), clamp(orgNormalColor.b*2.0 - 1.0, -1.0, 1.0)));
}

void directLight()
{			
	vec3 N = vec3(0.0f,0.0f,1.0f);
	if((u_textureFlags & 4 ) != 0)
		 N = textureToNormal(texture(texture2, texCoord0)); 
	vec3 L = normalize(lightDir);
N = vec3(0.0f,0.0f,1.0f);
	
	float lambertTerm = dot(N,L);

	if(lambertTerm > 0 && lambertTerm <= 1)
		out_Color = vec4(out_Color.xyz*(lambertTerm+0.5),out_Color.w);
	else out_Color = vec4(out_Color.xyz*0.5,out_Color.w);
	
	out_Color = vec4(texture(texture2, texCoord0).rgb,.0f);
}

void main()
{
	out_Color = vec4(1.0);
	
	/*if((u_textureFlags & 1 ) != 0)
		out_Color = texture(texture0, texCoord0);
		
	if((u_textureFlags & 2 ) != 0)
		out_Color = texture(texture1, texCoord0);
		
	if((u_textureFlags & 3 ) != 0)
	{
		vec4 water1 = texture(texture0, texCoord0);
		vec4 water2 = texture(texture1, texCoord0);
		out_Color = mix(water1, water2,1.0f);
	}*/
	
	vec3 worldIncident = normalize(u_dir);
	vec3 worldNormal = vec3(0,0,1.0);
	
	vec3 refraction = refract(worldIncident, worldNormal, Eta);
	vec3 reflection = reflect(worldIncident, worldNormal);
	
	vec4 refractionColor = texture(texture0, refraction);
	vec4 reflectionColor = texture(texture0, reflection);
	
	float fresnel = Eta + (1.0 - Eta) * pow(max(0.0, 1.0 - dot(-worldIncident, worldNormal)), 5.0);
	
	out_Color = mix(refractionColor, reflectionColor, fresnel);
	
	//directLight();
	

}

#endif