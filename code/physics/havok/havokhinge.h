#pragma once
//------------------------------------------------------------------------------
/**
    @class Havok::HavokHinge

	>> Work in progress, will not work! <<
    
    (C) 2013-2014 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------

#include "physics/joints/basehinge.h"

namespace Physics
{
	class PhysicsBody;
}

namespace Havok
{
class HavokHinge : public Physics::BaseHinge
{
	__DeclareClass(HavokHinge);
public:
	/// default constructor
	HavokHinge();
	/// destructor
	~HavokHinge();

	/// set up the joint with a pivot and an axis
	virtual void Setup(const Math::vector & pivot, const Math::vector & axis );
	/// set up the joint with two pivots and two axes
	virtual void Setup(const Math::vector & pivotA, const Math::vector & axisInA, const Math::vector & pivotB, const Math::vector & axisInB);

	/// set angle limits
	virtual void SetLimits(float low, float high);
	/// get the lower limit
	virtual float GetLowLimit();
	/// get the higher limit
	virtual float GetHighLimit();

	/// set joint axis
	virtual void SetAxis(const Math::vector & axis);

	/// set the angular motor
	virtual void SetAngularMotor(float targetVelocity, float maxImpulse);
	/// enable or disable the angular motor
	virtual void SetEnableAngularMotor(bool enable);

	/// get current angle
	virtual float GetHingeAngle();

	//------------------------------------------------------------------------------
	// base joint methods

	/// update position and orientation
	virtual void UpdateTransform(const Math::matrix44& m);
	/// render debug visualization
	virtual void RenderDebug();

	/// attach to scene
	virtual void Attach(Physics::BaseScene * world);
	/// detach from scene
	virtual void Detach();
	/// is attached
	virtual bool IsAttached() const;
	/// set enable
	virtual void SetEnabled(bool b);
	/// is enabled
	bool IsEnabled() const;
	/// get the first body
	const Ptr<Physics::PhysicsBody> & GetBody1() const;
	/// get the second body (can be null)
	const Ptr<Physics::PhysicsBody> & GetBody2() const;
	/// set link name (for linking to a character joint)
	void SetLinkName(const Util::String& n);
	/// get link name
	const Util::String& GetLinkName();
	/// return true if a link name has been set
	bool IsLinkValid() const;
	/// set link index
	void SetLinkIndex(int i);
	/// get link index
	int GetLinkIndex() const;
	/// set joint type
	void SetType(Physics::JointType  t);	
	/// set the 2 bodies connected by the joint (0 pointers are valid)
	void SetBodies(const Ptr<Physics::PhysicsBody> & body1, const Ptr<Physics::PhysicsBody> & body2);	

	/// set joint breaking threshold
	virtual void SetBreakThreshold(float threshold);
	/// get joint breaking threshold
	virtual float GetBreakThreshold();

	virtual void SetERP(float ERP, int axis = 0 );
	virtual void SetCFM(float CFM, int axis = 0);

	virtual void SetStoppingERP(float ERP, int axis = 0);
	virtual void SetStoppingCFM(float CFM, int axis = 0);

	virtual float GetERP(int axis = 0);
	virtual float GetCFM(int axis = 0);

	virtual float GetStoppingERP(int axis = 0);
	virtual float GetStoppingCFM(int axis = 0);
};
}