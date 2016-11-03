#pragma once
//------------------------------------------------------------------------------
/**
	@class Bullet::BulletUniversalJoint

	(C) 2012-2016 Individual contributors, see AUTHORS file
*/
#include "joints/baseuniversaljoint.h"
#include "base/basescene.h"

class btRigidBody;
class btDynamicsWorld;
class btTypedConstraint;

namespace Bullet
{

class BulletUniversalJoint: public Physics::BaseUniversalJoint
{
	__DeclareClass(BulletUniversalJoint);

public:
	/// default constructor
	BulletUniversalJoint();

	/// setup 
	void Setup(const Math::vector & anchor, const Math::vector & axisA, const Math::vector & axisB);
	
	///
	float GetAxisAngleA();
	///
	float GetAxisAngleB();
	
	///
	void SetLowLimits(float A, float B);
	///
	void SetHighLimits(float A, float B);	
};

}
