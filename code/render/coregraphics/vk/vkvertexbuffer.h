#pragma once
//------------------------------------------------------------------------------
/**
	Implements a vertex buffer in Vulkan.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"
#include "coregraphics/base/vertexbufferbase.h"
namespace Vulkan
{
class VkVertexBuffer : public Base::VertexBufferBase
{
	__DeclareClass(VkVertexBuffer);
public:
	/// constructor
	VkVertexBuffer();
	/// destructor
	virtual ~VkVertexBuffer();

	/// unload the resource, or cancel the pending load
	virtual void Unload();
	/// map the vertices for CPU access
	void* Map(MapType mapType);
	/// unmap the resource
	void Unmap();

	/// handle updating the vertex buffer
	void Update(const void* data, SizeT offset, SizeT length, void* mappedData = NULL);

	/// get vulkan buffer
	const VkBuffer& GetVkBuffer() const;
	/// set vulkan buffer and device memory
	void SetVkBuffer(const VkBuffer& buf, const VkDeviceMemory& mem);
private:
	VkDeviceMemory mem;
	VkBuffer buf;
	uint32_t mapcount;

	VkCommandBuffer updCmd;
	VkBufferMemoryBarrier barrier;
	VkMemoryBarrier memBarrier;
};

//------------------------------------------------------------------------------
/**
*/
inline const VkBuffer&
VkVertexBuffer::GetVkBuffer() const
{
	return this->buf;
}

//------------------------------------------------------------------------------
/**
*/
inline void
VkVertexBuffer::SetVkBuffer(const VkBuffer& buf, const VkDeviceMemory& mem)
{
	this->buf = buf;
	this->mem = mem;
}


} // namespace Vulkan