#pragma once
//------------------------------------------------------------------------------
/**
	An event is a subsystem implemented synchronization variable. This type of event
	is meant to put into a (one or several) GPU work queues.
	
	(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"
namespace Base
{
class EventBase : public Core::RefCounted
{
	__DeclareClass(EventBase);
public:
	/// constructor
	EventBase();
	/// destructor
	virtual ~EventBase();

	/// set to true if event should created in the signaled state
	void SetSignaled(bool b);
	/// setup event
	void Setup();
	/// discard event
	void Discard();

	/// signal
	void Signal();
	/// wait for event to be set
	void Wait();
	/// reset event
	void Reset();
protected:
	bool createSignaled;
};

//------------------------------------------------------------------------------
/**
*/
inline void
EventBase::SetSignaled(bool b)
{
	this->createSignaled = b;
}

} // namespace Base