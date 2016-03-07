#pragma once
//------------------------------------------------------------------------------
/**
    @class Minimap::MinimapPlugin
    
    (C) 2016 Individual contributors, see AUTHORS file
*/
#include "core/refcounted.h"
#include "timing/time.h"
#include "frame/framebatch.h"
#include "ui/base/uiplugin.h"
#include "math/bbox.h"
#include "resources/managedtexture.h"
#include "rocket/rocketserver.h"

//------------------------------------------------------------------------------
namespace Minimap
{
class MinimapPlugin : public Rocket::Core::EventListenerInstancer, public Rocket::Core::EventListener
{
    
public:
    /// constructor
    MinimapPlugin();
    /// destructor
    virtual ~MinimapPlugin();

    
	/// create instance of event
	virtual Rocket::Core::EventListener* InstanceEventListener(const Rocket::Core::String& value, Rocket::Core::Element* element);
	///
	virtual void ProcessEvent(Rocket::Core::Event& event);

	/// releases instancer
	virtual void Release();
};

} // namespace Minimap