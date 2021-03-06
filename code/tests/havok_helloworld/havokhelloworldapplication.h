#pragma once
//------------------------------------------------------------------------------
/**
    @class Tests::HavokHelloWorldApplication
    
    Hello-world application for Havok physics. Also connects to a Havok
	visual debugger.
    
    (C) 2013 gscept
*/

#include "appgame/gameapplication.h"
#include "application/appgame/gameapplication.h"
#include "graphicsfeature/graphicsfeatureunit.h"
#include "basegamefeature/statehandlers/gamestatehandler.h"
#include "basegamefeature/basegamefeatureunit.h"

#if __USE_PHYSICS__
#include "physicsfeature/physicsfeatureunit.h"
#include "stateobjectfeature/stateobjectfeatureunit.h"
#endif

#if !__USE_HAVOK__
#error What are you doing, N_BUILD_HAVOK must be set!
#endif

//------------------------------------------------------------------------------
namespace Tests
{
class HavokHelloWorldState;

class HavokHelloWorldApplication : public App::GameApplication
{
	__DeclareSingleton(HavokHelloWorldApplication);
public:
	/// constructor
	HavokHelloWorldApplication();
	/// destructor
	~HavokHelloWorldApplication();
	/// open the application
	virtual bool Open();
	/// close the application
	virtual void Close();

private:
	/// setup application state handlers
	void SetupStateHandlers();
	/// cleanup application state handlers
	void CleanupStateHandlers();
	/// setup game features
	void SetupGameFeatures();
	/// cleanup game features
	void CleanupGameFeatures();

	Ptr<BaseGameFeature::BaseGameFeatureUnit> baseFeature;
	Ptr<GraphicsFeature::GraphicsFeatureUnit> graphicsFeature;
	Ptr<PhysicsFeature::PhysicsFeatureUnit> physicsFeature;		

	Ptr<HavokHelloWorldState> havokState;
	
}; 
} 
// namespace Tests
//------------------------------------------------------------------------------