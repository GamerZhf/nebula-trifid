//------------------------------------------------------------------------------
//  ogl4depthstenciltarget.cc
//  (C) 2013 Gustav Sterbrant
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ogl4depthstenciltarget.h"
#include "coregraphics/displaydevice.h"
#include "ogl4renderdevice.h"
#include "ogl4types.h"


namespace OpenGL4
{
__ImplementClass(OpenGL4::OGL4DepthStencilTarget, 'O4DT', Base::DepthStencilTargetBase);

using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
OGL4DepthStencilTarget::OGL4DepthStencilTarget() :
	ogl4DepthStencilRenderbuffer(0)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
OGL4DepthStencilTarget::~OGL4DepthStencilTarget()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void 
OGL4DepthStencilTarget::Setup()
{
	n_assert(0 == this->ogl4DepthStencilRenderbuffer);
	
	// setup base class
	DepthStencilTargetBase::Setup();
	
	// if we have a relative size on the dept-stencil target, calculate actual size
	if (this->useRelativeSize)
	{
		DisplayDevice* displayDevice = DisplayDevice::Instance();
		this->SetWidth(SizeT(displayDevice->GetDisplayMode().GetWidth() * this->relWidth));
		this->SetHeight(SizeT(displayDevice->GetDisplayMode().GetHeight() * this->relHeight));
	}

	// setup multisampling
	this->SetupMultiSampleType();

	// generate render buffer for depth-stencil texture
	glGenRenderbuffers(1, &this->ogl4DepthStencilRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, this->ogl4DepthStencilRenderbuffer);
	if (this->msCount > 1)
	{
		glRenderbufferStorageMultisample(
			GL_RENDERBUFFER,
			this->msCount,
			GL_DEPTH24_STENCIL8,
			this->width,
			this->height);
	}
	else
	{
		glRenderbufferStorage(
			GL_RENDERBUFFER,
			GL_DEPTH24_STENCIL8,
			this->width,
			this->height);
	}
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	n_assert(GLSUCCESS);
}

//------------------------------------------------------------------------------
/**
*/
void 
OGL4DepthStencilTarget::Discard()
{
	n_assert(0 != this->ogl4DepthStencilRenderbuffer);
	DepthStencilTargetBase::Discard();
	glDeleteRenderbuffers(1, &this->ogl4DepthStencilRenderbuffer);
	this->ogl4DepthStencilRenderbuffer = 0;
	n_assert(GLSUCCESS);
}

//------------------------------------------------------------------------------
/**
*/
void 
OGL4DepthStencilTarget::BeginPass()
{
	n_assert(0 != this->ogl4DepthStencilRenderbuffer);
	DepthStencilTargetBase::BeginPass();
	this->Clear(this->clearFlags);
}

//------------------------------------------------------------------------------
/**
*/
void 
OGL4DepthStencilTarget::EndPass()
{
	n_assert(0 != this->ogl4DepthStencilRenderbuffer);
	DepthStencilTargetBase::EndPass();
}

//------------------------------------------------------------------------------
/**
	Note: assumes a framebuffer object with this depth-stencil target is currently bound
*/
void 
OGL4DepthStencilTarget::Clear(uint flags)
{
	n_assert(0 != this->ogl4DepthStencilRenderbuffer);
	if (flags & (ClearDepth | ClearStencil))
	{
		glDepthMask(GL_TRUE);
		glStencilMask(1);
		glClearBufferfi(GL_DEPTH_STENCIL, 0, this->clearDepth, this->clearStencil);
	}
	else
	{
		// clear depth and stencil separately
		if (flags & ClearDepth)
		{
			glDepthMask(GL_TRUE);
			glClearBufferfv(GL_DEPTH, 0, &this->clearDepth);
		}
		if (0 != (flags & ClearStencil))
		{
			glStencilMask(1);
			glClearBufferiv(GL_STENCIL, 0, &this->clearStencil);
		}
	}
	
}

//------------------------------------------------------------------------------
/**
*/
void 
OGL4DepthStencilTarget::OnDisplayResized(SizeT width, SizeT height)
{
	n_assert(0 != this->ogl4DepthStencilRenderbuffer);

	// if we have a relative size on the dept-stencil target, calculate actual size
	if (this->useRelativeSize)
	{		
		this->width = SizeT(Math::n_floor(width * this->relWidth));
		this->height = SizeT(Math::n_floor(height * this->relHeight));

		// bind buffer and bind a new storage
		glBindRenderbuffer(GL_RENDERBUFFER, this->ogl4DepthStencilRenderbuffer);

		if (this->msCount > 1)
		{
			glRenderbufferStorageMultisample(
				GL_RENDERBUFFER,
				this->msCount,
				GL_DEPTH24_STENCIL8,
				this->width,
				this->height);
		}
		else
		{
			glRenderbufferStorage(
				GL_RENDERBUFFER,
				GL_DEPTH24_STENCIL8,
				this->width,
				this->height);
		}
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		n_assert(GLSUCCESS);
	}
}

//------------------------------------------------------------------------------
/**
Select the antialias parameters that most closely resemble
the preferred settings in the DisplayDevice object.
*/
void
OGL4DepthStencilTarget::SetupMultiSampleType()
{
	n_assert(0 != this->format);
	OGL4RenderDevice* renderDevice = OGL4RenderDevice::Instance();

	// convert Nebula3 antialias quality into D3D type
	this->msCount = OGL4Types::AsOGL4MultiSampleType(this->antiAliasQuality);

	if (this->msCount > 0)
	{
		// check if the multisample type is compatible with the selected display mode
		GLuint availableQualityLevels = 0;
		GLuint depthBufferQualityLevels = 0;

		// clamp multisample quality to the available quality levels
		if (availableQualityLevels > 0)
		{
			this->msQuality = availableQualityLevels;
		}
		else
		{
			this->msQuality = 0;
		}
	}
}

} // namespace OpenGL4