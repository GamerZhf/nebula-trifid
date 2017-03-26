//------------------------------------------------------------------------------
/**
  blur_2d_cs.fxh

	Blurring kernel used for 2D textures. Implements a double pass X-Y blur with a defined kernel size.
	First pass samples from a render-able texture (Alt0) and works in the X-axis.
	The second pass resamples from the same texture and blurs in the Y-axis.


	Include this header and then define if you want an RGBA16F, RG16F or RG32F image as input.

  (C) 2016 Gustav Sterbrant
*/
#include "lib/shared.fxh"

#if IMAGE_IS_RGBA16F
#define IMAGE_FORMAT_TYPE rgba16f
#define IMAGE_LOAD_VEC vec4
#define IMAGE_LOAD_SWIZZLE(vec) vec.xyzw
#define RESULT_TO_VEC4(vec) vec
#elif IMAGE_IS_RGB16F
#define IMAGE_FORMAT_TYPE rgba16f
#define IMAGE_LOAD_VEC vec3
#define IMAGE_LOAD_SWIZZLE(vec) vec.xyz
#define RESULT_TO_VEC4(vec) vec4(vec, 1)
#elif IMAGE_IS_RG16F
#define IMAGE_FORMAT_TYPE rg16f
#define IMAGE_LOAD_VEC vec2
#define IMAGE_LOAD_SWIZZLE(vec) vec.xy
#define RESULT_TO_VEC4(vec) vec4(vec.xy, 0, 0)
#elif IMAGE_IS_RG32F
#define IMAGE_FORMAT_TYPE rg32f
#define IMAGE_LOAD_VEC vec2
#define IMAGE_LOAD_SWIZZLE(vec) vec.xy
#define RESULT_TO_VEC4(vec) vec4(vec.xy, 0, 0)
#endif

samplerstate InputSampler
{
	Filter = Point;
	AddressU = Clamp;
	AddressV = Clamp;
};

textureHandle InputImageX;
textureHandle InputImageY;
write IMAGE_FORMAT_TYPE image2D BlurImageX;
write IMAGE_FORMAT_TYPE image2D BlurImageY;
#define INV_LN2 1.44269504f
#define SQRT_LN2 0.832554611f
#define SIGMA 10.0f
#define INVSIGMA 2.0f


#define BLUR_KERNEL_32 1

#if BLUR_KERNEL_32
#define KERNEL_RADIUS 16
#define KERNEL_RADIUS_FLOAT 16.0f
#define HALF_KERNEL_RADIUS_FLOAT (KERNEL_RADIUS/2.0f)
#define HALF_KERNEL_RADIUS (KERNEL_RADIUS/2)
#define GAUSSIAN_KERNEL_SIZE_33 1
#elif BLUR_KERNEL_64
#define KERNEL_RADIUS 32
#define KERNEL_RADIUS_FLOAT 32.0f
#define HALF_KERNEL_RADIUS_FLOAT (KERNEL_RADIUS/2.0f)
#define HALF_KERNEL_RADIUS (KERNEL_RADIUS/2)
#define GAUSSIAN_KERNEL_SIZE_65 1
#endif

#define BLUR_TILE_WIDTH 320
#define SHARED_MEM_SIZE (KERNEL_RADIUS + BLUR_TILE_WIDTH + KERNEL_RADIUS)



const float weights[] = {
#if GAUSSIAN_KERNEL_SIZE_65
0.00024,0.000328,0.000445,0.000598,0.000795,0.001046,0.001363,0.001759,0.002246,0.002841,0.003557,0.00441,0.005412,0.006576,0.007912,0.009423,0.011112,0.012973,0.014996,0.017162,0.019445,0.021812,0.024225,0.026637,0.028998,0.031255,0.033352,0.035236,0.036857,0.038168,0.039134,0.039725,0.039924,0.039725,0.039134,0.038168,0.036857,0.035236,0.033352,0.031255,0.028998,0.026637,0.024225,0.021812,0.019445,0.017162,0.014996,0.012973,0.011112,0.009423,0.007912,0.006576,0.005412,0.00441,0.003557,0.002841,0.002246,0.001759,0.001363,0.001046,0.000795,0.000598,0.000445,0.000328,0.00024
#elif GAUSSIAN_KERNEL_SIZE_33
0.000485,0.000899,0.001603,0.002745,0.004519,0.007147,0.010863,0.015864,0.022263,0.030022,0.038903,0.048441,0.05796,0.066638,0.073622,0.078159,0.079733,0.078159,0.073622,0.066638,0.05796,0.048441,0.038903,0.030022,0.022263,0.015864,0.010863,0.007147,0.004519,0.002745,0.001603,0.000899,0.000485
#endif
};
groupshared IMAGE_LOAD_VEC SharedMemory[SHARED_MEM_SIZE];

//------------------------------------------------------------------------------
/**
*/
[localsizex] = SHARED_MEM_SIZE
shader
void
csMainX() 
{
	// get full resolution and inverse full resolution
	ivec2 size = imageSize(BlurImageX);
	
	// calculate offsets
	const uint         tileStart = int(gl_WorkGroupID.x) * BLUR_TILE_WIDTH;
	const uint           tileEnd = tileStart + BLUR_TILE_WIDTH;
	const uint        apronStart = tileStart - KERNEL_RADIUS;
	const uint          apronEnd = tileEnd   + KERNEL_RADIUS;
	
	const uint x = apronStart + gl_LocalInvocationID.x;
	const uint y = gl_WorkGroupID.y;
	const ivec2 sampleIndex = ivec2(x, y);
	const vec2 sampleCoord = sampleIndex / vec2(size);
	
	// load into workgroup saved memory, this allows us to use the original pixel even though 
	// we might have replaced it with the result from this thread!
	SharedMemory[gl_LocalInvocationID.x] = IMAGE_LOAD_SWIZZLE(sample2DLod(InputImageX, InputSampler, sampleCoord, 0));
	groupMemoryBarrier();
	
	const uint writePos = tileStart + gl_LocalInvocationID.x;
	const uint tileEndClamped = min(tileEnd, uint(size.x));
	
	if (writePos < tileEndClamped)
	{
		IMAGE_LOAD_VEC blurTotal = IMAGE_LOAD_VEC(0);
		
		int i;
		int weightIndex = 0;		
		#pragma unroll
		for (i = 0; i < KERNEL_RADIUS * 2; ++i)
		{
		    // Sample the pre-filtered data with step size = 2 pixels
		    uint j = 1 * uint(i) + gl_LocalInvocationID.x;
		    IMAGE_LOAD_VEC samp = SharedMemory[j];
			float weight = weights[i];
		    blurTotal += weight * samp;
		}
		
		IMAGE_LOAD_VEC color = blurTotal;
		imageStore(BlurImageX, ivec2(writePos, y), RESULT_TO_VEC4(color));
	}
}

//------------------------------------------------------------------------------
/**
*/
[localsizex] = SHARED_MEM_SIZE
shader
void
csMainY() 
{
	// get full resolution and inverse full resolution
	ivec2 size = imageSize(BlurImageY);
	
	// calculate offsets
	const uint         tileStart = int(gl_WorkGroupID.x) * BLUR_TILE_WIDTH;
	const uint           tileEnd = tileStart + BLUR_TILE_WIDTH;
	const uint        apronStart = tileStart - KERNEL_RADIUS;
	const uint          apronEnd = tileEnd   + KERNEL_RADIUS;
	
	const uint x = gl_WorkGroupID.y;
	const uint y = apronStart + gl_LocalInvocationID.x;
	const ivec2 sampleIndex = ivec2(x, y);
	const vec2 sampleCoord = sampleIndex / vec2(size);
	
	// load into workgroup saved memory, this allows us to use the original pixel even though 
	// we might have replaced it with the result from this thread!
	SharedMemory[gl_LocalInvocationID.x] = IMAGE_LOAD_SWIZZLE(sample2DLod(InputImageY, InputSampler, sampleCoord, 0));
	groupMemoryBarrier();
	
	const uint writePos = tileStart + gl_LocalInvocationID.x;
	const uint tileEndClamped = min(tileEnd, uint(size.y));
	
	if (writePos < tileEndClamped)
	{
		IMAGE_LOAD_VEC blurTotal = IMAGE_LOAD_VEC(0);
				
		int i;
		int weightIndex = 0;
		#pragma unroll
		for (i = 0; i < KERNEL_RADIUS * 2; ++i)
		{
		    // Sample the pre-filtered data with step size = 2 pixels
		    uint j = 1 * uint(i) + gl_LocalInvocationID.x;
		    IMAGE_LOAD_VEC samp = SharedMemory[j];
			float weight = weights[i];
		    blurTotal += weight * samp;
		}
			
		IMAGE_LOAD_VEC color = blurTotal;
		imageStore(BlurImageY, ivec2(x, writePos), RESULT_TO_VEC4(color));
	}
}