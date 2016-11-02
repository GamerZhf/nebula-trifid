//------------------------------------------------------------------------------
//  text.fx
//
//	Basic text shader
//
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------

#include "lib/std.fxh"
#include "lib/techniques.fxh"
#include "lib/shared.fxh"

/// Declaring used textures
group(SYSTEM_GROUP) push varblock Text
{
	textureHandle Texture;
	mat4 TextProjectionModel;
};

group(SYSTEM_GROUP) samplerstate TextureSampler
{
	Filter = Linear;
};

state TextState
{
	BlendEnabled[0] = true;
	SrcBlend[0] = SrcAlpha;
	DstBlend[0] = OneMinusSrcAlpha;
	DepthWrite = false;
	DepthEnabled = false;
	CullMode = None;
	ScissorEnabled = true;
};

//------------------------------------------------------------------------------
/**
*/
shader
void
vsMain(
	[slot=0] in vec2 position,
	[slot=1] in vec2 uv,
	[slot=2] in vec4 color,
	out vec2 UV,
	out vec4 Color) 
{
	vec4 pos = vec4(position, 0, 1);	
	gl_Position = Text.TextProjectionModel * pos;
	Color = color;
	UV = uv;
}

//------------------------------------------------------------------------------
/**
*/
shader
void
psMain(
	in vec2 UV,
	in vec4 Color,
	[color0] out vec4 FinalColor) 
{
	vec4 texColor = sample2D(Text.Texture, TextureSampler, UV).rrrr;
	FinalColor = texColor * Color;
}

//------------------------------------------------------------------------------
/**
*/
SimpleTechnique(Default, "Static", vsMain(), psMain(), TextState);