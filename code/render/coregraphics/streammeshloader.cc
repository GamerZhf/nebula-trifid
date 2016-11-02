//------------------------------------------------------------------------------
//  streammeshloader.cc
//  (C) 2007 Radon Labs GmbH
//  (C) 2013-2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/streammeshloader.h"

#if __DX11__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::StreamMeshLoader, 'SMLD', Direct3D11::D3D11StreamMeshLoader);
}
#elif __DX9__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::StreamMeshLoader, 'SMLD', Win360::D3D9StreamMeshLoader);
}
#elif __OGL4__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::StreamMeshLoader, 'SMLD', OpenGL4::OGL4StreamMeshLoader);
}
#elif __VULKAN__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::StreamMeshLoader, 'SMLD', Vulkan::VkStreamMeshLoader);
}
#elif __WII__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::StreamMeshLoader, 'SMLD', Wii::WiiStreamMeshLoader);
}
#elif __PS3__
namespace CoreGraphics
{
__ImplementClass(CoreGraphics::StreamMeshLoader, 'SMLD', Resources::StreamResourceLoader);
}
#else
#error "StreamMeshLoader class not implemented on this platform!"
#endif
