//------------------------------------------------------------------------------
//  skybox.fx
//  (C) 2012 Gustav Sterbrant
//------------------------------------------------------------------------------

#include "lib/std.fxh"
#include "lib/shared.fxh"
#include "lib/util.fxh"
#include "lib/techniques.fxh"
#include "lib/preetham.fxh"

float Contrast = 1.0f;
float Brightness = 1.0f;
float SkyBlendFactor = 0.0f;
float SkyRotationFactor = 0.03;

#define PI_HALF 1.5707963267948966192313216916398f

// declare two textures, one main texture and one blend texture together with a wrapping sampler
samplerCube SkyLayer1;
samplerCube SkyLayer2;

samplerstate SkySampler
{
	Samplers = { SkyLayer1, SkyLayer2 };
	AddressU = Wrap; 
	AddressV = Wrap;
	AddressW = Wrap;
	Filter = MinMagLinearMipPoint;
};

state SkyboxState
{
	CullMode = Front;
	DepthEnabled = true;
	DepthWrite = false;
	DepthFunc = Equal;
};

//------------------------------------------------------------------------------
/**
    Sky box vertex shader
*/
shader
void
vsMain(in vec3 position,
	in vec3 normal,
	in vec2 uv,
	in vec3 tangent,
	in vec3 binormal,
	out vec3 UV,
	out vec3 Direction)
{
	vec3 tempPos = normalize(position);
	gl_Position = Projection * vec4(tempPos, 1);	
	float animationSpeed = TimeAndRandom.x * SkyRotationFactor;	
	mat3 rotMat = mat3( cos(animationSpeed), 0, sin(animationSpeed),
						0, 1, 0,
						-sin(animationSpeed), 0, cos(animationSpeed));
						
	float3 viewSample = (InvView * vec4(tempPos, 0)).xyz;
	Direction = viewSample;
	UV = viewSample * rotMat;
}

//------------------------------------------------------------------------------
/**
    Skybox pixel shader
*/
shader
void
psMain(in vec3 UV,
	in vec3 Direction,
	[color0] out vec4 Color)
{
	vec3 lightDir = normalize(GlobalLightDirWorldspace.xyz);
	vec3 dir = normalize(Direction);
	vec3 atmo = Preetham(dir, lightDir, A, B, C, D, E) * GlobalLightColor.rgb;
	
	// rotate uvs around center with constant speed
	vec3 baseColor = textureLod(SkyLayer1, UV, 0).rgb;
	vec3 blendColor = textureLod(SkyLayer2, UV, 0).rgb;
	vec3 color = mix(baseColor, blendColor, SkyBlendFactor);		
	color = ((color - 0.5f) * Contrast) + 0.5f;
	color *= Brightness;	
	color *= atmo * ONE_OVER_PI;
		
	Color = EncodeHDR(vec4(color, 1));
	gl_FragDepth = 1.0f;
}

//------------------------------------------------------------------------------
/**
*/
SimpleTechnique(Default, "Static", vsMain(), psMain(), SkyboxState);