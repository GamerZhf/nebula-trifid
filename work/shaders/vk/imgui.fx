//------------------------------------------------------------------------------
//  imgui.fx
//
//	Shader for ImGUI rendering
//
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------

#include "lib/std.fxh"
#include "lib/techniques.fxh" 
#include "lib/shared.fxh"
#include "lib/defaultsamplers.fxh"

// put variables in push-constant block
group(SYSTEM_GROUP) push varblock ImGUI
{
	textureHandle Texture; 
	mat4 TextProjectionModel;
};

group(SYSTEM_GROUP) samplerstate TextureSampler
{
	Filter = Point;
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
	gl_Position = ImGUI.TextProjectionModel * pos;
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
	vec4 texColor = sample2D(ImGUI.Texture, TextureSampler, UV);
	FinalColor = Color * texColor;
}

//------------------------------------------------------------------------------
/**
*/
SimpleTechnique(Default, "Static", vsMain(), psMain(), TextState);
