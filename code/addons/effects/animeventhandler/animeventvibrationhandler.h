#pragma once
//------------------------------------------------------------------------------
/**
    @class FX::AnimEventVibrationHandler
    
    This is the godsend animevent handler, to handle hotspot triggered vibrations
    
    (C) 2009 Radon Labs GmbH
	(C) 2013-2016 Individual contributors, see AUTHORS file
*/
#include "animation/animeventhandlerbase.h"
#include "io/uri.h"
#include "timing/timer.h"

//------------------------------------------------------------------------------
namespace EffectsFeature
{
class AnimEventVibrationHandler : public Animation::AnimEventHandlerBase
{
    __DeclareClass(AnimEventVibrationHandler);

public:
    /// constructor
    AnimEventVibrationHandler();
    /// destructor
    virtual ~AnimEventVibrationHandler();  

    /// handle a event
    virtual bool HandleEvent(const Animation::AnimEventInfo& event);
};
} // namespace EffectsFeature
//------------------------------------------------------------------------------