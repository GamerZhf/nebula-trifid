#pragma once
//------------------------------------------------------------------------------
/**
	Implements a command buffer thread which incrementally writes to Vulkan command buffers.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <vulkan/vulkan.h>
#include "threading/thread.h"
#include "threading/safequeue.h"
#include "coregraphics/primitivegroup.h"
#include "debug/debugtimer.h"
namespace Vulkan
{
class VkCmdBufferThread : public Threading::Thread
{
	__DeclareClass(VkCmdBufferThread);
	struct Command;
public:
	/// constructor
	VkCmdBufferThread();
	/// destructor
	virtual ~VkCmdBufferThread();

	/// called if thread needs a wakeup call before stopping
	void EmitWakeupSignal();
	/// this method runs in the thread context
	void DoWork();
	/// push command buffer work
	void PushCommand(const Command& command);
	/// push command buffer work
	void PushCommands(const Util::Array<Command>& commands);
	/// set command buffer
	void SetCommandBuffer(const VkCommandBuffer& buffer);
private:
	friend class VkRenderDevice;

	enum CommandType
	{
		BeginCommand,
		EndCommand,
		GraphicsPipeline,
		ComputePipeline,
		InputAssemblyVertex,
		InputAssemblyIndex,
		Draw,
		Dispatch,
		BindDescriptors,
		PushRange,
		Viewport,
		ViewportArray,
		ScissorRect,
		ScissorRectArray,
		UpdateBuffer,
		SetEvent,					// sets event to flagged
		ResetEvent,					// resets event to unflagged
		WaitForEvent,
		Barrier,
		Sync,
		LunarGCircumventValidation
	};

	struct Command
	{
		CommandType type;

		union
		{
			VkPipeline pipeline;

			struct // BeginCmd
			{
				VkCommandBufferBeginInfo info;
				VkCommandBuffer buf;
			} bgCmd;

			struct // VBO
			{
				VkBuffer buffer;
				IndexT index;
				VkDeviceSize offset;
			} vbo;

			struct // IBO
			{
				VkBuffer buffer;
				VkDeviceSize offset;
				VkIndexType indexType;
			} ibo;			
			
			struct // Draw
			{
				uint32_t baseIndex;
				uint32_t baseVertex;
				uint32_t numIndices;
				uint32_t numVerts;
				uint32_t baseInstance;
				uint32_t numInstances;
			} draw;

			struct // Dispatch
			{
				uint32_t numGroupsX;
				uint32_t numGroupsY;
				uint32_t numGroupsZ;
			} dispatch;

			struct // Descriptors
			{
				VkPipelineBindPoint type;
				VkPipelineLayout layout;
				uint32_t baseSet;
				uint32_t numSets;
				const VkDescriptorSet* sets;
				uint32_t numOffsets;
				const uint32_t* offsets;
			} descriptor;

			struct // UpdateBuffer
			{
				bool deleteWhenDone;
				VkBuffer buf;
				VkDeviceSize offset;
				VkDeviceSize size;
				uint32_t* data;
			} updBuffer;

			struct // PushConstants
			{
				VkShaderStageFlags stages;
				VkPipelineLayout layout;
				uint32_t offset;
				uint32_t size;
				void* data;
			} pushranges;

			struct // Viewport
			{
				VkViewport vp;
				uint32_t index;
			} viewport;

			struct // ViewportArray
			{
				VkViewport* vps;
				uint32_t first;
				uint32_t num;
			} viewportArray;

			struct // ScissorRect
			{
				VkRect2D sc;
				uint32_t index;
			} scissorRect;

			struct // ScissorRectArray
			{
				VkRect2D* scs;
				uint32_t first;
				uint32_t num;
			} scissorRectArray;

			struct // SetEvent
			{
				VkEvent event;
				VkPipelineStageFlags stages;
			} setEvent;

			struct // ResetEvent
			{
				VkEvent event;
				VkPipelineStageFlags stages;
			} resetEvent;

			struct // WaitForEvents
			{
				VkEvent* events;
				uint32_t numEvents;
				VkPipelineStageFlags signalingStage;
				VkPipelineStageFlags waitingStage;
				uint32_t memoryBarrierCount;
				VkMemoryBarrier* memoryBarriers;
				uint32_t bufferBarrierCount;
				VkBufferMemoryBarrier* bufferBarriers;
				uint32_t imageBarrierCount;
				VkImageMemoryBarrier* imageBarriers;
			} waitEvent;

			struct // Barrier
			{
				VkPipelineStageFlags srcMask;
				VkPipelineStageFlags dstMask;
				VkDependencyFlags dep;
				uint32_t memoryBarrierCount;
				VkMemoryBarrier* memoryBarriers;
				uint32_t bufferBarrierCount;
				VkBufferMemoryBarrier* bufferBarriers;
				uint32_t imageBarrierCount;
				VkImageMemoryBarrier* imageBarriers;
			} barrier;

			Threading::Event* syncEvent;
		};		
	};

	VkCommandBuffer commandBuffer;
	Threading::SafeQueue<Command> commands;
#if NEBULA3_ENABLE_PROFILING
	_declare_timer(debugTimer);
#endif
};


//------------------------------------------------------------------------------
/**
*/
inline void
VkCmdBufferThread::SetCommandBuffer(const VkCommandBuffer& buffer)
{
	this->commandBuffer = buffer;
}

//------------------------------------------------------------------------------
/**
*/
inline void
VkCmdBufferThread::PushCommand(const Command& command)
{
	this->commands.Enqueue(command);
}

//------------------------------------------------------------------------------
/**
*/
inline void
VkCmdBufferThread::PushCommands(const Util::Array<Command>& commands)
{
	this->commands.EnqueueArray(commands);
}

} // namespace Vulkan