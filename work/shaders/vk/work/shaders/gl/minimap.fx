//------------------------------------------------------------------------------
//  minimap.fx
//  (C) 2016 Johannes Hirche
//------------------------------------------------------------------------------
#include "lib/std.fxh"
#include "lib/shared.fxh"
#include "lib/geometrybase.fxh"
#include "lib/techniques.fxh"

sampler2D Portrait;

state MinimapState
{
	BlendEnabled[0] = true;
	SrcBlend[0] = SrcAlpha;
	DstBlend[0] = OneMinusSrcAlpha;
	CullMode = None;
	DepthEnabled = false;
	DepthWrite = false;
};

// samplers
samplerstate PortraitSampler
{
	Samplers = { Portrait };
};

//varblock MinimapBlock 
//{	
	mat4 TransArray[MAX_BATCH_SIZE];
	vec4 ColorArray[MAX_BATCH_SIZE];	
//};

//------------------------------------------------------------------------------
/**
*/
shader
void
vsMinimap(
	in vec2 uv,
	out vec2 UV) 
{	
    gl_Position = TransArray[gl_InstanceID] * vec4(uv,0,1);		
    UV = uv;
}

//------------------------------------------------------------------------------
/**
*/
shader
void
psMinimap(in vec2 UV,	
	[color0] out vec4 FinalColor) 
{
	vec4 portraitColor = texture(Portrait,UV);
	FinalColor = portraitColor;//vec4(0.5,0.5, portraitColor.r, 1);
}

//------------------------------------------------------------------------------
/**
*/
SimpleTechnique(minimap, "Static", vsMinimap(), psMinimap(), MinimapState);