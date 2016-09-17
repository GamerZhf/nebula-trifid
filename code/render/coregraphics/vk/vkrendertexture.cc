//------------------------------------------------------------------------------
// vkrendertexture.cc
// (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "vkrendertexture.h"
#include "vktypes.h"
#include "vkrenderdevice.h"

using namespace CoreGraphics;
namespace Vulkan
{

__ImplementClass(Vulkan::VkRenderTexture, 'VKRT', Base::RenderTextureBase);
//------------------------------------------------------------------------------
/**
*/
VkRenderTexture::VkRenderTexture() :
	img(VK_NULL_HANDLE),
	mem(VK_NULL_HANDLE),
	view(VK_NULL_HANDLE)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
VkRenderTexture::~VkRenderTexture()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
VkRenderTexture::Setup()
{
	RenderTextureBase::Setup();

	// if this is a window texture, get the backbuffers from the render device
	if (this->windowTexture)
	{
		this->swapimages = VkRenderDevice::Instance()->backbuffers;
		VkClearColorValue clear = { 0, 0, 0, 0 };

		VkImageSubresourceRange subres;
		subres.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subres.baseArrayLayer = 0;
		subres.baseMipLevel = 0;
		subres.layerCount = 1;
		subres.levelCount = 1;

		// clear textures
		IndexT i;
		for (i = 0; i < this->swapimages.Size(); i++)
		{
			VkRenderDevice::Instance()->PushImageLayoutTransition(VkDeferredCommand::Graphics, VkRenderDevice::ImageMemoryBarrier(this->swapimages[i], subres, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL));
			VkRenderDevice::Instance()->PushImageColorClear(this->swapimages[i], VkDeferredCommand::Graphics, VK_IMAGE_LAYOUT_GENERAL, clear, subres);
			VkRenderDevice::Instance()->PushImageLayoutTransition(VkDeferredCommand::Graphics, VkRenderDevice::ImageMemoryBarrier(this->swapimages[i], subres, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL));
		}

		// setup texture
		this->texture->SetupFromVkBackbuffer(this->swapimages[0], this->width, this->height, this->depth, this->format);
	}
	else
	{
		VkSampleCountFlagBits sampleCount = this->msaaEnabled ? VK_SAMPLE_COUNT_16_BIT : VK_SAMPLE_COUNT_1_BIT;

		VkExtent3D extents;
		extents.width = this->width;
		extents.height = this->height;
		extents.depth = this->depth;

		VkImageViewType viewType;
		switch (this->type)
		{
		case Texture::Texture2D:
			viewType = VK_IMAGE_VIEW_TYPE_2D;
			break;
		case Texture::Texture2DArray:
			viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
			break;
		case Texture::TextureCube:
			viewType = VK_IMAGE_VIEW_TYPE_CUBE;
			break;
		case Texture::TextureCubeArray:
			viewType = VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
			break;
		}

        VkFormat fmt = VkTypes::AsVkFramebufferFormat(this->format);
        if (this->usage == ColorAttachment)
        {
            VkFormatProperties formatProps;
            vkGetPhysicalDeviceFormatProperties(VkRenderDevice::physicalDev, fmt, &formatProps);
            n_assert(formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT &&
                     formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT &&
                     formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);
        }
		VkImageUsageFlags usageFlags = this->usage == ColorAttachment ? VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT : VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		VkImageCreateInfo imgInfo =
		{
			VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			NULL,
			0,
			VK_IMAGE_TYPE_2D,   // if we have a cube map, it's just 2D * 6
			fmt,
			extents,
			1,
			this->layers,
			sampleCount,
			VK_IMAGE_TILING_OPTIMAL,
			usageFlags | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT,
			VK_SHARING_MODE_EXCLUSIVE,
			0,
			NULL,
			VK_IMAGE_LAYOUT_UNDEFINED
		};

		// create image for rendering
		VkResult res = vkCreateImage(VkRenderDevice::dev, &imgInfo, NULL, &this->img);
		n_assert(res == VK_SUCCESS);

		// allocate buffer backing and bind to image
		uint32_t size;
		VkRenderDevice::Instance()->AllocateImageMemory(this->img, this->mem, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, size);
		vkBindImageMemory(VkRenderDevice::dev, this->img, this->mem, 0);

		VkImageAspectFlags aspect = this->usage == ColorAttachment ? VK_IMAGE_ASPECT_COLOR_BIT : VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
		VkImageSubresourceRange subres;
		subres.baseArrayLayer = 0;
		subres.baseMipLevel = 0;
		subres.layerCount = 1;
		subres.levelCount = 1;
		subres.aspectMask = aspect;
 		VkImageViewCreateInfo viewInfo =
		{
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			NULL,
			0,
			this->img,
			viewType,
			VkTypes::AsVkFramebufferFormat(this->format),
			VkTypes::AsVkMapping(this->format),
			subres
		};

		res = vkCreateImageView(VkRenderDevice::dev, &viewInfo, NULL, &this->view);
		n_assert(res == VK_SUCCESS);

		if (this->usage == ColorAttachment)
		{
			// setup actual texture
			if (sampleCount > 1)
			{
				this->texture->SetupFromVkMultisampleTexture(this->img, this->mem, this->view, this->width, this->height, this->format, 0, true, true);
			}
			else
			{
				this->texture->SetupFromVkTexture(this->img, this->mem, this->view, this->width, this->height, this->format, 0, true, true);
			}

			// clear image and transition layout
			VkClearColorValue clear = { 0, 0, 0, 0 };
			VkRenderDevice::Instance()->PushImageLayoutTransition(VkDeferredCommand::Graphics, VkRenderDevice::ImageMemoryBarrier(this->img, subres, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL));
			VkRenderDevice::Instance()->PushImageColorClear(this->img, VkDeferredCommand::Graphics, VK_IMAGE_LAYOUT_GENERAL, clear, subres);
			VkRenderDevice::Instance()->PushImageLayoutTransition(VkDeferredCommand::Graphics, VkRenderDevice::ImageMemoryBarrier(this->img, subres, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
		}
		else
		{
			// clear image and transition layout
			VkClearDepthStencilValue clear = { 1, 0 };
			VkRenderDevice::Instance()->PushImageLayoutTransition(VkDeferredCommand::Graphics, VkRenderDevice::ImageMemoryBarrier(this->img, subres, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL));
			VkRenderDevice::Instance()->PushImageDepthStencilClear(this->img, VkDeferredCommand::Graphics, VK_IMAGE_LAYOUT_GENERAL, clear, subres);
			VkRenderDevice::Instance()->PushImageLayoutTransition(VkDeferredCommand::Graphics, VkRenderDevice::ImageMemoryBarrier(this->img, subres, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL));

			// setup actual texture
			if (sampleCount > 1)
			{
				this->texture->SetupFromVkMultisampleTexture(this->img, this->mem, this->view, this->width, this->height, this->format, 0, true, true);
			}
			else
			{
				this->texture->SetupFromVkTexture(this->img, this->mem, this->view, this->width, this->height, this->format, 0, true, true);
			}
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void
VkRenderTexture::Resize()
{

}

//------------------------------------------------------------------------------
/**
*/
void
VkRenderTexture::SwapBuffers()
{
	RenderTextureBase::SwapBuffers();
	VkRenderDevice* dev = VkRenderDevice::Instance();
	VkResult res = vkAcquireNextImageKHR(dev->dev, dev->swapchain, UINT64_MAX, dev->displaySemaphore, VK_NULL_HANDLE, &dev->currentBackbuffer);
	if (res == VK_ERROR_OUT_OF_DATE_KHR)
	{
		// this means our swapchain needs a resize!
	}
	else
	{
		n_assert(res == VK_SUCCESS);
	}

	// set image and update texture
	this->img = this->swapimages[dev->currentBackbuffer];
	this->texture->img = this->img;
}


} // namespace Vulkan