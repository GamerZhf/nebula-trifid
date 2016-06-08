//------------------------------------------------------------------------------
//  lightmapbase.fxh
//  (C) 2015 Gustav Sterbrant
//------------------------------------------------------------------------------

#ifndef LIGHTMAPBASE_FXH
#define LIGHTMAPBASE_FXH

/// Declaring used textures
sampler2D LightMap;
float LightMapIntensity = 0.0f;

samplerstate LightmapSampler
{
	Samplers = { SpecularMap, EmissiveMap, NormalMap, LightMap, AlbedoMap };
};

//------------------------------------------------------------------------------
/**
	Pixel shader for lightmapped lit geometry
*/
shader
void
psLightmappedLit(in vec3 ViewSpacePos,
	in vec3 Tangent,
	in vec3 Normal,
	in vec3 Binormal,
	in vec2 UV1,
	in vec2 UV2,
	[color0] out vec4 Albedo,
	[color1] out vec4 Normals,
	[color2] out float Depth,
	[color3] out vec4 Specular) 
{
	vec4 diffColor = texture(AlbedoMap, UV1);
	vec4 emsvColor = EncodeHDR(texture(EmissiveMap, UV1));
	vec4 specColor = texture(SpecularMap, UV1);
	float roughness = texture(RoughnessMap, UV1).r;
	vec4 lightMapColor = vec4(((texture(LightMap, UV2.xy) - 0.5f) * 2.0f * LightMapIntensity).rgb, 1);
	
	Specular = vec4(specColor.rgb * MatSpecularIntensity.rgb, roughness);
	Albedo = diffColor * lightMapColor;
	Depth = length(ViewSpacePos.xyz);
	mat3 tangentViewMatrix = mat3(normalize(Tangent.xyz), normalize(Binormal.xyz), normalize(Normal.xyz));        
	vec3 tNormal = vec3(0,0,0);
	tNormal.xy = (texture(NormalMap, UV1).ag * 2.0) - 1.0;
	tNormal.z = saturate(sqrt(1.0 - dot(tNormal.xy, tNormal.xy)));
	
	Normals = PackViewSpaceNormal((tangentViewMatrix * tNormal).xyz);

	float alpha = diffColor.a;
	if (alpha < AlphaSensitivity) discard;
}

//------------------------------------------------------------------------------
/**
	Pixel shader for lightmapped lit geometry together with vertex colors
*/
shader
void
psLightmappedLitVertexColors(in vec3 ViewSpacePos,
	in vec3 Tangent,
	in vec3 Normal,
	in vec3 Binormal,
	in vec2 UV1,
	in vec2 UV2,
	in vec4 Color,
	[color0] out vec4 Albedo,
	[color1] out vec4 Normals,
	[color2] out float Depth,
	[color3] out vec4 Specular) 
{
	vec4 diffColor = texture(AlbedoMap, UV1);
	vec4 emsvColor = EncodeHDR(texture(EmissiveMap, UV1));
	vec4 specColor = texture(SpecularMap, UV1);
	float roughness = texture(RoughnessMap, UV1).r;
	vec4 lightMapColor = vec4(((texture(LightMap, UV2.xy) - 0.5f) * 2.0f * LightMapIntensity).rgb, 1);
	
	Specular = vec4(specColor.rgb * MatSpecularIntensity.rgb, roughness);
	Albedo = diffColor * Color * lightMapColor;
	Depth = length(ViewSpacePos.xyz);
	mat3 tangentViewMatrix = mat3(normalize(Tangent.xyz), normalize(Binormal.xyz), normalize(Normal.xyz));        
	vec3 tNormal = vec3(0,0,0);
	tNormal.xy = (texture(NormalMap, UV1).ag * 2.0) - 1.0;
	tNormal.z = saturate(sqrt(1.0 - dot(tNormal.xy, tNormal.xy)));
	
	Normals = PackViewSpaceNormal((tangentViewMatrix * tNormal).xyz);

	float alpha = diffColor.a;
	if (alpha < AlphaSensitivity) discard;
}

//------------------------------------------------------------------------------
/**
	Pixel shader for lightmapped unlit geometry
*/
shader
void
psLightmappedUnlit(in vec2 UV1,
	in vec2 UV2,
	[color0] out vec4 Albedo) 
{
	vec4 diffColor = texture(AlbedoMap, UV1.xy);
	vec4 lightMapColor = vec4(((texture(LightMap, UV2.xy) - 0.5f) * 2.0f * LightMapIntensity).rgb, 1);
	
	Albedo = diffColor * lightMapColor;
	
	float alpha = diffColor.a;
	if (alpha < AlphaSensitivity) discard;
}

//------------------------------------------------------------------------------
/**
	Pixel shader for lightmapped unlit geometry with vertex colors
*/
shader
void
psLightmappedUnlitVertexColors(in vec2 UV1,
	in vec2 UV2,
	in vec4 Color,
	[color0] out vec4 Albedo) 
{
	vec4 diffColor = texture(AlbedoMap, UV1.xy);
	vec4 lightMapColor = vec4(((texture(LightMap, UV2.xy) - 0.5f) * 2.0f * LightMapIntensity).rgb, 1);
	
	Albedo = diffColor * lightMapColor * Color;

	float alpha = diffColor.a;
	if (alpha < AlphaSensitivity) discard;
}

#endif