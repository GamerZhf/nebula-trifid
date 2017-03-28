//------------------------------------------------------------------------------
//  imguiconsolehandler.cc
//  (C) 2015-2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "imguiconsolehandler.h"
#include "io/console.h"

namespace Dynui
{
__ImplementClass(Dynui::ImguiConsoleHandler, 'IMCH', IO::ConsoleHandler);

//------------------------------------------------------------------------------
/**
*/
ImguiConsoleHandler::ImguiConsoleHandler() 
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
ImguiConsoleHandler::~ImguiConsoleHandler()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
ImguiConsoleHandler::Setup()
{
	n_assert(IO::Console::HasInstance());
	IO::Console::Instance()->AttachHandler(this);
}

//------------------------------------------------------------------------------
/**
*/
void
ImguiConsoleHandler::Discard()
{
	IO::Console::Instance()->RemoveHandler(this);
}

//------------------------------------------------------------------------------
/**
*/
void
ImguiConsoleHandler::Open()
{
	n_assert(ImguiConsole::HasInstance());	
	ConsoleHandler::Open();
}

//------------------------------------------------------------------------------
/**
*/
void
ImguiConsoleHandler::Close()
{
	ConsoleHandler::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
ImguiConsoleHandler::Print(const Util::String& msg)
{
	ImguiConsole::Instance()->AppendToLog(msg);
}

//------------------------------------------------------------------------------
/**
*/
void
ImguiConsoleHandler::Error(const Util::String& msg)
{
	ImguiConsole::Instance()->AppendToLog(msg);
}
//------------------------------------------------------------------------------
/**
*/
void
ImguiConsoleHandler::Warning(const Util::String& msg)
{
	ImguiConsole::Instance()->AppendToLog(msg);
}
//------------------------------------------------------------------------------
/**
*/
void
ImguiConsoleHandler::DebugOut(const Util::String& msg)
{
	ImguiConsole::Instance()->AppendToLog(msg);
}


} // namespace Dynui