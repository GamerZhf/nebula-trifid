#pragma once
//------------------------------------------------------------------------------
/**
    Platform-wrapper for memory texture loader
    
    (C) 2012 Johannes Hirche
*/
#if __DX11__
#include "coregraphics/d3d11/d3d11memorytextureloader.h"
namespace CoreGraphics
{
class MemoryTextureLoader : public Direct3D11::D3D11MemoryTextureLoader
{
	__DeclareClass(MemoryTextureLoader);
};
}
#elif __OGL4__
#include "coregraphics/ogl4/ogl4memorytextureloader.h"
namespace CoreGraphics
{
class MemoryTextureLoader : public OpenGL4::OGL4MemoryTextureLoader
{
	__DeclareClass(MemoryTextureLoader);
};
}
#elif __VULKAN__
#include "coregraphics/vk/vkmemorytextureloader.h"
namespace CoreGraphics
{
class MemoryTextureLoader : public Vulkan::VkMemoryTextureLoader
{
	__DeclareClass(MemoryTextureLoader);
};
}
#elif __DX9__
#include "coregraphics/d3d9/d3d9memorytextureloader.h"
namespace CoreGraphics
{
class MemoryTextureLoader : public Direct3D9::D3D9MemoryTextureLoader
{
	__DeclareClass(MemoryTextureLoader);
};
}
#else
#error "memorytextureloader is not implemented on this configuration"
#endif