#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::RenderShape
    
    Describes a shape which is rendered through the ShapeRenderer singleton.
    Shape rendering may be relatively inefficient and should only be used
    for debug visualizations.
    Please note that vertex and index data will be copied into a memory
    stream, so that it is safe to release or alter the original data
    once the shape object has been created. You have to be aware of
    the performance and memory-footprint implications though.
    Shape objects can be copied, but they will share the internal 
    vertex/index data copy.
    
    (C) 2008 Radon Labs GmbH
    (C) 2013-2016 Individual contributors, see AUTHORS file
*/
#include "math/matrix44.h"
#include "math/float4.h"
#include "coregraphics/primitivetopology.h"
#include "coregraphics/indextype.h"
#include "io/memorystream.h"
#include "threading/threadid.h"
#include "mesh.h"

//------------------------------------------------------------------------------
namespace CoreGraphics
{
class RenderShape
{
public:
    /// shape types
    enum Type
    {
        Box,
        Sphere,
        Cylinder,
        Torus,
		Cone,
        Primitives,
        IndexedPrimitives,
		RenderMesh,

        NumShapeTypes,
        InvalidShapeType,
    };

	enum RenderFlag
	{
		CheckDepth = 0,
		AlwaysOnTop,
		Wireframe,

		NumDepthFlags
	};

    struct RenderShapeVertex
    {
        Math::float4 pos;
        Math::float4 color;

        RenderShapeVertex() : color(Math::float4(1)) {}
    };

    /// default constructor
    RenderShape();
    /// shortcut constructor for simple shapes
    RenderShape(Threading::ThreadId threadId, Type shapeType, RenderFlag depthFlag, const Math::matrix44& modelTransform, const Math::float4& color);
    /// return true if object has been setup
    bool IsValid() const;
    /// setup simple shape
    void SetupSimpleShape(Threading::ThreadId threadId, Type shapeType, RenderFlag depthFlag, const Math::matrix44& modelTransform, const Math::float4& color);

    /// setup primitive batch (SLOW!)
    void SetupPrimitives(Threading::ThreadId threadId, 
            const Math::matrix44& modelTransform, 
            PrimitiveTopology::Code topology, 
            SizeT numPrimitives, 
            const RenderShape::RenderShapeVertex* vertices,
            const Math::float4& color, 
            RenderFlag depthFlag);

    /// setup indexed primitive batch (SLOW!)
    void SetupIndexedPrimitives(Threading::ThreadId threadId, 
        const Math::matrix44& modelTransform, 
        PrimitiveTopology::Code topology, 
        SizeT numPrimitives, 
        const RenderShape::RenderShapeVertex* vertices,
        SizeT numVertices, 
        const void* indices, 
        IndexType::Code indexType, 
        const Math::float4& color, 
        RenderFlag depthFlag);

	/// setup mesh
	void SetupMesh(Threading::ThreadId threadId, const Math::matrix44& modelTransform, const Ptr<Mesh>& mesh, const IndexT groupIndex, const Math::float4& color, RenderFlag depthFlag);

    /// get the thread id of this shape
    Threading::ThreadId GetThreadId() const;
    /// get shape type
    Type GetShapeType() const;
    /// get depth flag
	RenderFlag GetDepthFlag() const;
	/// get model transform
    const Math::matrix44& GetModelTransform() const;
    /// get primitive topology
    PrimitiveTopology::Code GetTopology() const;
    /// get number of primitives
    SizeT GetNumPrimitives() const;
    /// get pointer to vertex data (returns 0 if none exist)
    const void* GetVertexData() const;
    /// get vertex width in number of floats
    SizeT GetVertexWidth() const;
    /// get number of vertices (only for indexed primitives)
    SizeT GetNumVertices() const;
    /// get index data (returns 0 if none exists)
    const void* GetIndexData() const;
    /// get the index type (16 or 32 bit)
    IndexType::Code GetIndexType() const;
    /// get shape color
    const Math::float4& GetColor() const;
    /// get vertex layout, returns NULL if none exist
    const Ptr<VertexLayout>& GetVertexLayout() const;
	/// get mesh
	const Ptr<Mesh>& GetMesh() const;
	/// get primitive group
	const IndexT& GetPrimitiveGroupIndex() const;

private:
    Threading::ThreadId threadId;
    Type shapeType;
	RenderFlag depthFlag;
    Math::matrix44 modelTransform;
    PrimitiveTopology::Code topology;
    SizeT numPrimitives;
    SizeT vertexWidth;
    SizeT numVertices;
    IndexType::Code indexType;
    Math::float4 color;
    IndexT vertexDataOffset;

	IndexT groupIndex;
	Ptr<Mesh> mesh;
    Ptr<VertexLayout> vertexLayout;
    Ptr<IO::MemoryStream> dataStream;       // contains vertex/index data
};

//------------------------------------------------------------------------------
/**
*/
inline bool
RenderShape::IsValid() const
{
    return (InvalidShapeType != this->shapeType);
}

//------------------------------------------------------------------------------
/**
*/
inline Threading::ThreadId
RenderShape::GetThreadId() const
{
    return this->threadId;
}

//------------------------------------------------------------------------------
/**
*/
inline RenderShape::Type
RenderShape::GetShapeType() const
{
    return this->shapeType;
}

//------------------------------------------------------------------------------
/**
*/
inline RenderShape::RenderFlag
RenderShape::GetDepthFlag() const
{
	return this->depthFlag;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
RenderShape::GetModelTransform() const
{
    return this->modelTransform;
}

//------------------------------------------------------------------------------
/**
*/
inline PrimitiveTopology::Code
RenderShape::GetTopology() const
{
    n_assert((Primitives == this->shapeType) || (IndexedPrimitives == this->shapeType));
    return this->topology;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
RenderShape::GetNumPrimitives() const
{
    n_assert((Primitives == this->shapeType) || (IndexedPrimitives == this->shapeType));
    return this->numPrimitives;
}

//------------------------------------------------------------------------------
/**
*/
inline const void*
RenderShape::GetVertexData() const
{
    n_assert((Primitives == this->shapeType) || (IndexedPrimitives == this->shapeType));
    const void* ptr = ((uchar*)this->dataStream->GetRawPointer()) + this->vertexDataOffset;
    return ptr;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
RenderShape::GetVertexWidth() const
{
    n_assert((Primitives == this->shapeType) || (IndexedPrimitives == this->shapeType));
    return this->vertexWidth;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
RenderShape::GetNumVertices() const
{
    n_assert(IndexedPrimitives == this->shapeType);
    return this->numVertices;
}

//------------------------------------------------------------------------------
/**
*/
inline const void*
RenderShape::GetIndexData() const
{
    n_assert(IndexedPrimitives == this->shapeType);
    const void* ptr = this->dataStream->GetRawPointer();
    return ptr;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexType::Code
RenderShape::GetIndexType() const
{
    n_assert(IndexedPrimitives == this->shapeType);
    return this->indexType;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4&
RenderShape::GetColor() const
{
    return this->color;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<VertexLayout>&
RenderShape::GetVertexLayout() const
{
    return this->vertexLayout;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Mesh>& 
RenderShape::GetMesh() const
{
	return this->mesh;
}

//------------------------------------------------------------------------------
/**
*/
inline const IndexT&
RenderShape::GetPrimitiveGroupIndex() const
{
	return this->groupIndex;
}

} // namespace CoreGraphics
//------------------------------------------------------------------------------

