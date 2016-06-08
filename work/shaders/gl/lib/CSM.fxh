//------------------------------------------------------------------------------
//  CSM.fxh
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------

#ifndef CSM_FXH
#define CSM_FXH

#define PCF 0
#define BLURSAMPLES 1
#define NO_COMPARISON 1

#include "shadowbase.fxh"
//shared varblock CSMParamBlock
//{
	vec4 CascadeOffset[CASCADE_COUNT_FLAG];
	vec4 CascadeScale[CASCADE_COUNT_FLAG];
	float MinBorderPadding;     
	float MaxBorderPadding;
	float ShadowPartitionSize; 
	float GlobalLightShadowBias = 0.0f;
	
//};
	
const int SplitsPerRow = 2;
const int SplitsPerColumn = 2;

const vec4 DebugColors[8] = 
{
    float4 ( 1.5f, 0.0f, 0.0f, 1.0f ),
    float4 ( 0.0f, 1.5f, 0.0f, 1.0f ),
    float4 ( 0.0f, 0.0f, 5.5f, 1.0f ),
    float4 ( 1.5f, 0.0f, 5.5f, 1.0f ),
    float4 ( 1.5f, 1.5f, 0.0f, 1.0f ),
    float4 ( 1.0f, 1.0f, 1.0f, 1.0f ),
    float4 ( 0.0f, 1.0f, 5.5f, 1.0f ),
    float4 ( 0.5f, 3.5f, 0.75f, 1.0f )
};

sampler2D ShadowProjMap;            

samplerstate ShadowProjMapSampler
{
	Samplers = { ShadowProjMap };
	Filter = MinMagMipLinear;
	//AddressU = Border;
	//AddressV = Border;
	//MaxAnisotropic = 16;
	//BorderColor = { 1,1,1,1 };
};

const float CascadeBlendArea = 0.1f;
//------------------------------------------------------------------------------
/**
*/
vec4 ConvertViewRayToWorldPos(vec3 viewRay, float length, vec3 cameraPosition)
{
	return vec4(cameraPosition + viewRay * length, 1);
}

//------------------------------------------------------------------------------
/**
	Converts World Position into shadow texture lookup vector, modelviewprojection position and interpolated position, as well as depth
*/
void CSMConvert(in vec4 worldPosition,	
				out vec4 texShadow)
{       
    // Transform the shadow texture coordinates for all the cascades.
    texShadow = CSMShadowMatrix * worldPosition;
}

const vec2 sampleOffsets[] = {
	vec2(-.326,-.406),
	vec2(-.840,-.074),
	vec2(-.696, .457),
	vec2(-.203, .621),
	vec2( .962,-.195),
	vec2( .473,-.480),
	vec2( .519, .767),
	vec2( .185,-.893),
	vec2( .507, .064),
	vec2( .896, .412),
	vec2(-.322,-.933),
	vec2(-.792,-.598)
};

const float MomentBiases[] =
{
	2e-5f,
	3e-5f,
	4e-4f,
	4e-4f
};

const float DepthBiases[] = 
{
	2e-5f,
	3e-5f,
	4e-4f,
	4e-4f
};

//------------------------------------------------------------------------------
/**
*/
void 
CalculateBlendAmountForMap ( in vec4 texCoord, 
                             out float blendBandLocation,
                             out float blendAmount ) 
{
    // calculate the blend band for the map based selection.
    vec2 distanceToOne = vec2(1.0f - texCoord.x, 1.0f - texCoord.y);
    blendBandLocation = min(texCoord.x, texCoord.y);
    float blendBandLocation2 = min(distanceToOne.x, distanceToOne.y);
    blendBandLocation = min(blendBandLocation, blendBandLocation2);
    blendAmount = blendBandLocation / (CascadeBlendArea - 0.05f);
}

//------------------------------------------------------------------------------
/**
	CSM shadow sampling entry point
*/
float
CSMPS(in vec4 TexShadow,
	  in vec2 FSUV,
	  out vec4 Debug)
{
	vec4 texCoordShadow = vec4(0.0f);
    
    float fPercentLit = 1.0f;
	bool cascadeFound = false;
	float bias = GlobalLightShadowBias;
    vec4 texCoordViewspace = TexShadow;// / TexShadow.wwww;
	
	int cascadeIndex;
	for( cascadeIndex = 0; cascadeIndex < CASCADE_COUNT_FLAG; ++cascadeIndex) 
	{
		texCoordShadow = texCoordViewspace * CascadeScale[cascadeIndex] + CascadeOffset[cascadeIndex];
		if ( min( texCoordShadow.x, texCoordShadow.y ) > MinBorderPadding
		  && max( texCoordShadow.x, texCoordShadow.y ) < MaxBorderPadding )
		{ 
			cascadeFound = true;
			break;
		}
	}
	
	float blendAmount = 0;
	float blendBandLocation = 0;
	CalculateBlendAmountForMap(texCoordShadow, blendBandLocation, blendAmount);
				
	// if we have no matching cascade, return with a fully lit pixel
	if (!cascadeFound)
	{
		return 1.0f;
	}		
	
	// calculate texture coordinate in shadow space
	vec2 texCoord = texCoordShadow.xy / texCoordShadow.w;
	float depth = texCoordShadow.z;

	vec2 sampleCoord = texCoord;
	sampleCoord.xy *= ShadowPartitionSize;
	sampleCoord.xy += vec2((cascadeIndex % SplitsPerRow) * ShadowPartitionSize, (cascadeIndex / SplitsPerColumn) * ShadowPartitionSize);

	// get pixel size of shadow projection texture
	vec4 mapDepth = texture(ShadowProjMap, sampleCoord);
	float occlusion = MSMShadowSample(mapDepth, 5e-5f, depth, 0);
	//float occlusion = ExponentialShadowSample(mapDepth, depth, 0.0f);
		
	int nextCascade = min(CASCADE_COUNT_FLAG-1, cascadeIndex+1);;
	float occlusionBlend = 1.0f;
	if (blendBandLocation < CascadeBlendArea)
	{
		//if (nextCascade < CASCADE_COUNT_FLAG)
		{
			texCoordShadow = texCoordViewspace * CascadeScale[nextCascade];
			texCoordShadow += CascadeOffset[nextCascade];
			
			texCoord = texCoordShadow.xy;
			depth = texCoordShadow.z;
			
			sampleCoord = texCoord;			
			sampleCoord.xy *= ShadowPartitionSize;
			sampleCoord.xy += vec2((nextCascade % SplitsPerRow) * ShadowPartitionSize, (nextCascade / SplitsPerColumn) * ShadowPartitionSize);
								
			mapDepth = texture(ShadowProjMap, sampleCoord);
			occlusionBlend = MSMShadowSample(mapDepth, 5e-5f, depth, 0);
		}
		
		// blend next cascade onto previous
		occlusion = lerp(occlusionBlend, occlusion, blendAmount);
	}
	//occlusion += smoothstep(0.98f, 1.0f, increment);
	//occlusion += increment;
	
	
	// finally clamp all shadow values 0.5, this avoids any weird color differences when blending between cascades
	//Debug = DebugColors[cascadeIndex];
	return occlusion;
	//return occlusion;
}

#endif