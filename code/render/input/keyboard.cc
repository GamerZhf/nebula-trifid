//------------------------------------------------------------------------------
//  keyboard.cc
//  (C) 2007 Radon Labs GmbH
//  (C) 2013-2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "input/keyboard.h"
#if __WIN32__ || __LINUX__
namespace Input
{
__ImplementClass(Input::Keyboard, 'KBRD', Base::KeyboardBase);
}
#else
#error "Keyboard class not implemented on this platform!"
#endif
