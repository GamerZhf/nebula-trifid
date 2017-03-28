#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::StreamShaderLoader
    
    Resource loader to setup a Shader object from a stream.
    
    (C) 2007 Radon Labs GmbH
    (C) 2013-2016 Individual contributors, see AUTHORS file
*/
#if __DX11__
#include "coregraphics/d3d11/d3d11streamshaderloader.h"
namespace CoreGraphics
{
class StreamShaderLoader : public Direct3D11::D3D11StreamShaderLoader
{
	__DeclareClass(StreamShaderLoader);
};
}
#elif __OGL4__
#include "coregraphics/ogl4/ogl4streamshaderloader.h"
namespace CoreGraphics
{
class StreamShaderLoader : public OpenGL4::OGL4StreamShaderLoader
{
	__DeclareClass(StreamShaderLoader);
};
}
#elif __VULKAN__
#include "coregraphics/vk/vkstreamshaderloader.h"
namespace CoreGraphics
{
class StreamShaderLoader : public Vulkan::VkStreamShaderLoader
{
	__DeclareClass(StreamShaderLoader);
};
}
#elif __DX9__
#include "coregraphics/d3d9/d3d9streamshaderloader.h"
namespace CoreGraphics
{
class StreamShaderLoader : public Direct3D9::D3D9StreamShaderLoader
{
    __DeclareClass(StreamShaderLoader);
};
}
#else
#error "StreamShaderLoader class not implemented on this platform!"
#endif
//------------------------------------------------------------------------------

    