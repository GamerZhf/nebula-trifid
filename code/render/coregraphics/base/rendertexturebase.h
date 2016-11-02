#pragma once
//------------------------------------------------------------------------------
/**
	A render texture is a special kind of texture which may be used for rendering to
	in a framebuffer, or sample in a shader. 

	Internally, the render texture is just an ordinary texture, however setup doesn't
	require any data to initialize it.

	FIXME: Good opportunity to use the new window system now to make the screen relative
	bool use the proper window pointer.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"
#include "coregraphics/texture.h"
#include "coregraphics/pixelformat.h"
namespace Base
{
class RenderTextureBase : public Core::RefCounted
{
	__DeclareClass(RenderTextureBase);
public:
	enum Usage
	{
		ColorAttachment,
		DepthStencilAttachment,

		InvalidAttachment
	};

	/// constructor
	RenderTextureBase();
	/// destructor
	virtual ~RenderTextureBase();

	/// set pixel format
	void SetPixelFormat(const CoreGraphics::PixelFormat::Code format);
	/// get pixel format
	const CoreGraphics::PixelFormat::Code& GetPixelFormat() const;
	/// set texture type, only 2D and Cube textures are allowed
	void SetTextureType(const CoreGraphics::Texture::Type& type);
	/// set how render texture is to be used
	void SetUsage(const Usage& usage);
	/// set dimensions
	void SetDimensions(const float width, const float height, const float depth = 1);
	/// get width
	const SizeT GetWidth() const;
	/// get height
	const SizeT GetHeight() const;
	/// get depth
	const SizeT GetDepth() const;
	/// set layers
	void SetLayers(const SizeT layers);
	/// get layers
	const SizeT GetLayers() const;
	/// set if render texture should be relative to screen, dimensions are used as multipliers
	void SetIsScreenRelative(const bool b);
	/// set if render texture should be dynamically scaled
	void SetIsDynamicScaled(const bool b);
	/// set if render texture should support MSAA
	void SetEnableMSAA(const bool b);
	/// get if texture is created as a multisample texture
	const bool GetEnableMSAA() const;
	/// set if render texture will go to a window
	void SetIsWindowTexture(const bool b);
	/// set resource id
	void SetResourceId(const Resources::ResourceId& resId);

	/// setup render texture
	void Setup();
	/// discard texture
	void Discard();
	/// resize render texture, retaining the same texture object
	void Resize();

	/// swap buffers, only valid if this is a window texture
	void SwapBuffers();

	/// called after we change the display size
	void OnDisplayResized(SizeT width, SizeT height);

	/// get texture
	const Ptr<CoreGraphics::Texture>& GetTexture() const;
protected:
	Ptr<CoreGraphics::Texture> texture;
	CoreGraphics::PixelFormat::Code format;
	CoreGraphics::Texture::Type type;
	Usage usage;

	bool msaaEnabled;
	bool relativeSize;
	bool dynamicSize;
	bool windowTexture;

	SizeT width;
	SizeT height;
	SizeT depth;
	SizeT layers;
	float widthScale;
	float heightScale;
	float depthScale;

	Resources::ResourceId resourceId;
};

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTextureBase::SetPixelFormat(const CoreGraphics::PixelFormat::Code format)
{
	this->format = format;
}

//------------------------------------------------------------------------------
/**
*/
inline const CoreGraphics::PixelFormat::Code&
RenderTextureBase::GetPixelFormat() const
{
	return this->format;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTextureBase::SetTextureType(const CoreGraphics::Texture::Type& type)
{
	this->type = type;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTextureBase::SetUsage(const Usage& usage)
{
	this->usage = usage;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTextureBase::SetDimensions(const float width, const float height, const float depth)
{
	this->width = (SizeT)width;
	this->height = (SizeT)height;
	this->depth = (SizeT)depth;
	this->widthScale = width;
	this->heightScale = height;
	this->depthScale = depth;
}

//------------------------------------------------------------------------------
/**
*/
inline const SizeT
RenderTextureBase::GetWidth() const
{
	return this->width;
}

//------------------------------------------------------------------------------
/**
*/
inline const SizeT
RenderTextureBase::GetHeight() const
{
	return this->height;
}

//------------------------------------------------------------------------------
/**
*/
inline const SizeT
RenderTextureBase::GetDepth() const
{
	return this->depth;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTextureBase::SetLayers(const SizeT layers)
{
	this->layers = layers;
}

//------------------------------------------------------------------------------
/**
*/
inline const SizeT
RenderTextureBase::GetLayers() const
{
	return this->layers;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTextureBase::SetIsScreenRelative(const bool b)
{
	this->relativeSize = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTextureBase::SetIsDynamicScaled(const bool b)
{
	this->dynamicSize = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTextureBase::SetEnableMSAA(const bool b)
{
	this->msaaEnabled = b;
}

//------------------------------------------------------------------------------
/**
*/
inline const bool
RenderTextureBase::GetEnableMSAA() const
{
	return this->msaaEnabled;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTextureBase::SetIsWindowTexture(const bool b)
{
	this->windowTexture = b;
}

//------------------------------------------------------------------------------
/**
*/
inline void
RenderTextureBase::SetResourceId(const Resources::ResourceId& resId)
{
	this->resourceId = resId;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::Texture>&
RenderTextureBase::GetTexture() const
{
	return this->texture;
}

} // namespace Base
