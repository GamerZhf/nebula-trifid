//------------------------------------------------------------------------------
//  tcpclient.cc
//  (C) 2009 Radon Labs GmbH
//  (C) 2013-2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "net/tcpclient.h"

namespace Net
{
#if (__WIN32__ || __XBOX360__ || __PS3__ || linux)
__ImplementClass(Net::TcpClient, 'TCPC', Net::StdTcpClient);
#elif __WII__
__ImplementClass(Net::TcpClient, 'TCPC', Core::RefCounted);
#else
#error "Net::TcpClient not implemented on this platform!"
#endif
}