#pragma once
//------------------------------------------------------------------------------
/**
    @class Base::ShapeRendererBase
    
    Base class of ShapeRenderer, can render a number of shapes, mainly 
    for debug visualization.
    
    (C) 2007 Radon Labs GmbH
    (C) 2013-2016 Individual contributors, see AUTHORS file
*/
#include "core/refcounted.h"
#include "core/singleton.h"
#include "math/matrix44.h"
#include "math/float4.h"
#include "math/bbox.h"
#include "coregraphics/primitivetopology.h"
#include "coregraphics/indextype.h"
#include "coregraphics/shaderstate.h"
#include "coregraphics/rendershape.h"
#include "threading/threadid.h"

//------------------------------------------------------------------------------
namespace Base
{
class ShapeRendererBase : public Core::RefCounted
{
    __DeclareClass(ShapeRendererBase);
    __DeclareSingleton(ShapeRendererBase);
public:
    /// constructor
    ShapeRendererBase();
    /// destructor
    virtual ~ShapeRendererBase();
    
    /// open the shape renderer
    void Open();
    /// close the shape renderer
    void Close();
    /// return true if open
    bool IsOpen() const;

    /// delete shapes of given thread id
    void DeleteShapesByThreadId(Threading::ThreadId threadId);
    /// add a shape for deferred rendering (can be called from outside render loop)
    void AddShape(const CoreGraphics::RenderShape& shape);
    /// add multiple shapes
    void AddShapes(const Util::Array<CoreGraphics::RenderShape>& shapeArray);
    /// add wireframe box
    void AddWireFrameBox(const Math::bbox& boundingBox, const Math::float4& color, Threading::ThreadId threadId);
    /// draw deferred shapes and clear deferred stack, must be called inside render loop
    void DrawShapes();

protected:
    bool isOpen;
	Util::Array<CoreGraphics::RenderShape> shapes[CoreGraphics::RenderShape::NumDepthFlags];
	Util::Array<CoreGraphics::RenderShape> primitives[CoreGraphics::RenderShape::NumDepthFlags];
};

//------------------------------------------------------------------------------
/**
*/
inline bool
ShapeRendererBase::IsOpen() const
{
    return this->isOpen;
}

} // namespace Base
//------------------------------------------------------------------------------


    