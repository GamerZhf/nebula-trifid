//------------------------------------------------------------------------------
//  red.fx
//  (C) 2016 Gustav Sterbrant
//------------------------------------------------------------------------------

#include "lib/std.fxh"
#include "lib/util.fxh"
#include "lib/techniques.fxh"
#include "lib/shared.fxh"

//sampler2D CopyBuffer;
textureHandle TestHandle;

samplerstate CopySampler 
{
//	Samplers = { CopyBuffer };
	Filter = Point;
};

state CopyState
{
	CullMode = None;
	DepthEnabled = false;
	DepthWrite = false;
};

//------------------------------------------------------------------------------
/**
*/
shader
void
vsMain(
	[slot=0] in vec3 position,
	[slot=2] in vec2 uv,
	out vec2 UV) 
{
    gl_Position = vec4(position, 1);
    UV = uv;
}

//------------------------------------------------------------------------------
/**
*/
shader
void
psMain(in vec2 uv, 
	[color0] out vec4 Color)
{
	Color = subpassLoad(InputAttachments[5]); 
}

//------------------------------------------------------------------------------
/**
*/
PostEffect(vsMain(), psMain(), CopyState);
