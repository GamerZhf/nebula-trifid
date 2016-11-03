#pragma once
//------------------------------------------------------------------------------
/**
    @class Physics::BaseStatic

    (C) 2013-2016 Individual contributors, see AUTHORS file
*/
#include "physicsobject.h"
#include "collider.h"
#include "math/matrix44.h"

namespace Physics
{
class Scene;

class BaseStatic : public PhysicsObject
{
__DeclareClass(BaseStatic);

public:
	BaseStatic(){}
	~BaseStatic(){}
	
protected:	
	friend class PhysicsObject;
	virtual void SetupFromTemplate(const PhysicsCommon & templ);

};
}