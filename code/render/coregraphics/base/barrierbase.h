#pragma once
//------------------------------------------------------------------------------
/**
	A barrier contains information about a GPU execution barrier, which is used to
	sequence two dependent commands.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"
namespace CoreGraphics
{
	class RenderTexture;
	class ShaderReadWriteTexture;
	class ShaderReadWriteBuffer;
}
namespace Base
{

class BarrierBase : public Core::RefCounted
{
	__DeclareClass(BarrierBase);
public:

	enum class Domain
	{
		Global,
		Pass
	};

	enum class Dependency
	{
		NoDependencies  = (1 << 0),
		VertexShader	= (1 << 1),		// blocks vertex shader
		HullShader		= (1 << 2),		// blocks hull (tessellation control) shader
		DomainShader	= (1 << 3),		// blocks domain (tessellation evaluation) shader
		GeometryShader	= (1 << 4),		// blocks geometry shader
		PixelShader		= (1 << 5),		// blocks pixel shader
		ComputeShader	= (1 << 6),		// blocks compute shaders to complete

		VertexInput		= (1 << 7),		// blocks vertex input
		EarlyDepth		= (1 << 8),		// blocks early fragment test
		LateDepth		= (1 << 9),		// blocks late fragment test

		Transfer		= (1 << 10),	// blocks transfers
		Host			= (1 << 11),	// blocks host operations
		PassOutput		= (1 << 12),	// blocks outputs from render texture attachments		

		Top				= (1 << 13),	// blocks start of pipeline
		Bottom			= (1 << 14)		// blocks end of pipeline
	};

	enum class Access
	{
		NoAccess				= (1 << 0),
		IndirectRead			= (1 << 1),	// indirect buffers are read
		IndexRead				= (1 << 2),	// index buffers are read
		VertexRead				= (1 << 3), // vertex buffers are read
		UniformRead				= (1 << 4), // uniforms are read
		InputAttachmentRead		= (1 << 5), // input attachments (cross-pass attachments) are read
		ShaderRead				= (1 << 6), // shader reads  (compute?)
		ShaderWrite				= (1 << 7), // shader writes (compute?)
		ColorAttachmentRead		= (1 << 8), // color attachments (render textures) are read
		ColorAttachmentWrite	= (1 << 9), // color attachments (render textures) are written
		DepthRead				= (1 << 10), // depth-stencil attachments are read
		DepthWrite				= (1 << 11), // depth-stencil attachments are written
		TransferRead			= (1 << 12), // transfers are read
		TransferWrite			= (1 << 13), // transfers are written
		HostRead				= (1 << 14), // host reads
		HostWrite				= (1 << 15), // host writes
		MemoryRead				= (1 << 16), // memory is read locally
		MemoryWrite				= (1 << 17)  // memory is written locally
	};	

	/// constructor
	BarrierBase();
	/// destructor
	virtual ~BarrierBase();

	/// set barrier domain
	void SetDomain(const Domain domain);
	/// get barrier domain
	const Domain GetDomain() const;
	/// set left-side dependency
	void SetLeftDependency(const Dependency dep);
	/// get left-side dependency
	const Dependency GetLeftDependency() const;
	/// set right-side dependency
	void SetRightDependency(const Dependency dep);
	/// get right-side dependency
	const Dependency GetRightDependency() const;
	/// add render target to block
	void AddRenderTexture(const Ptr<CoreGraphics::RenderTexture>& tex, Access leftAccess, Access rightAccess);
	/// get render textures array
	const Util::Array<Ptr<CoreGraphics::RenderTexture>>& GetRenderTextures() const;
	/// add read-write texture to block
	void AddReadWriteTexture(const Ptr<CoreGraphics::ShaderReadWriteTexture>& tex, Access leftAccess, Access rightAccess);
	/// get read-write textures array
	const Util::Array<Ptr<CoreGraphics::ShaderReadWriteTexture>>& GetReadWriteTextures() const;
	/// add read-write buffer to block
	void AddReadWriteBuffer(const Ptr<CoreGraphics::ShaderReadWriteBuffer>& buf, Access leftAccess, Access rightAccess);
	/// get read-write buffers array
	const Util::Array<Ptr<CoreGraphics::ShaderReadWriteBuffer>>& GetReadWriteBuffers() const;

	/// setup barrier
	void Setup();
	/// discard barrier
	void Discard();

	/// convert string to dependency
	static Dependency DependencyFromString(const Util::String& str);
	/// convert string to dependency
	static Access AccessFromString(const Util::String& str);
protected:

	Domain domain;
	Dependency leftDependency;
	Dependency rightDependency;
	Util::Array<Ptr<CoreGraphics::RenderTexture>> renderTextures;
	Util::Array<std::tuple<Access, Access>> renderTexturesAccess;
	Util::Array<Ptr<CoreGraphics::ShaderReadWriteTexture>> readWriteTextures;
	Util::Array<std::tuple<Access, Access>> readWriteTexturesAccess;
	Util::Array<Ptr<CoreGraphics::ShaderReadWriteBuffer>> readWriteBuffers;
	Util::Array<std::tuple<Access, Access>> readWriteBuffersAccess;
};

__ImplementEnumOperators(Base::BarrierBase::Dependency);
__ImplementEnumOperators(Base::BarrierBase::Access);
//------------------------------------------------------------------------------
/**
*/
inline void
BarrierBase::SetDomain(const Domain domain)
{
	this->domain = domain;
}

//------------------------------------------------------------------------------
/**
*/
inline const Base::BarrierBase::Domain
BarrierBase::GetDomain() const
{
	return this->domain;
}

//------------------------------------------------------------------------------
/**
*/
inline void
BarrierBase::SetLeftDependency(const Dependency dep)
{
	this->leftDependency = dep;
}

//------------------------------------------------------------------------------
/**
*/
inline const Base::BarrierBase::Dependency
BarrierBase::GetLeftDependency() const
{
	return this->leftDependency;
}

//------------------------------------------------------------------------------
/**
*/
inline void
BarrierBase::SetRightDependency(const Dependency dep)
{
	this->rightDependency = dep;
}


//------------------------------------------------------------------------------
/**
*/
inline const Base::BarrierBase::Dependency
BarrierBase::GetRightDependency() const
{
	return this->rightDependency;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<CoreGraphics::RenderTexture>>&
BarrierBase::GetRenderTextures() const
{
	return this->renderTextures;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<CoreGraphics::ShaderReadWriteTexture>>&
BarrierBase::GetReadWriteTextures() const
{
	return this->readWriteTextures;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Ptr<CoreGraphics::ShaderReadWriteBuffer>>&
BarrierBase::GetReadWriteBuffers() const
{
	return this->readWriteBuffers;
}

//------------------------------------------------------------------------------
/**
*/
inline Base::BarrierBase::Dependency
BarrierBase::DependencyFromString(const Util::String& str)
{
	if (str == "VertexShader")			return BarrierBase::Dependency::VertexShader;
	else if (str == "HullShader")		return BarrierBase::Dependency::HullShader;
	else if (str == "DomainShader")		return BarrierBase::Dependency::DomainShader;
	else if (str == "GeometryShader")	return BarrierBase::Dependency::GeometryShader;
	else if (str == "PixelShader")		return BarrierBase::Dependency::PixelShader;
	else if (str == "ComputeShader")	return BarrierBase::Dependency::ComputeShader;
	else if (str == "VertexInput")		return BarrierBase::Dependency::VertexInput;
	else if (str == "EarlyDepth")		return BarrierBase::Dependency::EarlyDepth;
	else if (str == "LateDepth")		return BarrierBase::Dependency::LateDepth;
	else if (str == "Transfer")			return BarrierBase::Dependency::Transfer;
	else if (str == "Host")				return BarrierBase::Dependency::Host;
	else if (str == "PassOutput")		return BarrierBase::Dependency::PassOutput;
	else if (str == "Top")				return BarrierBase::Dependency::Top;
	else if (str == "Bottom")			return BarrierBase::Dependency::Bottom;
	else
	{
		n_error("Invalid dependency string '%s'\n", str.AsCharPtr());
		return BarrierBase::Dependency::NoDependencies;
	}
}

//------------------------------------------------------------------------------
/**
*/
inline Base::BarrierBase::Access
BarrierBase::AccessFromString(const Util::String& str)
{
	if (str == "IndirectRead")					return BarrierBase::Access::IndirectRead;
	else if (str == "IndexRead")				return BarrierBase::Access::IndexRead;
	else if (str == "VertexRead")				return BarrierBase::Access::VertexRead;
	else if (str == "UniformRead")				return BarrierBase::Access::UniformRead;
	else if (str == "InputAttachmentRead")		return BarrierBase::Access::InputAttachmentRead;
	else if (str == "ShaderRead")				return BarrierBase::Access::ShaderRead;
	else if (str == "ShaderWrite")				return BarrierBase::Access::ShaderWrite;
	else if (str == "ColorAttachmentRead")		return BarrierBase::Access::ColorAttachmentRead;
	else if (str == "ColorAttachmentWrite")		return BarrierBase::Access::ColorAttachmentWrite;
	else if (str == "DepthRead")				return BarrierBase::Access::DepthRead;
	else if (str == "DepthWrite")				return BarrierBase::Access::DepthWrite;
	else if (str == "TransferRead")				return BarrierBase::Access::TransferRead;
	else if (str == "TransferWrite")			return BarrierBase::Access::TransferWrite;
	else if (str == "HostRead")					return BarrierBase::Access::HostRead;
	else if (str == "HostWrite")				return BarrierBase::Access::HostWrite;
	else if (str == "MemoryRead")				return BarrierBase::Access::MemoryRead;
	else if (str == "MemoryWrite")				return BarrierBase::Access::MemoryWrite;
	else
	{
		n_error("Invalid access string '%s'\n", str.AsCharPtr());
		return BarrierBase::Access::NoAccess;
	}
}

} // namespace CoreGraphics
