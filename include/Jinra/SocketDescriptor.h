#pragma once

namespace Jinra
{

/// Describes the local socket to use for NetPeer::Startup
struct JINRA_DLL_EXPORT SocketDescriptor
{
	SocketDescriptor();

	SocketDescriptor(u16 port, cchar* hostAddress);

	// The local port to bind to. Pass 0 to have the OS autoassign a port.
	u16 port;

	// The local network card address to bind to, such as "127.0.0.1".
	// Pass an empty string to use INADDR_ANY.
	char hostAddress[32];

	// IP version: For IPV4, use AF_INET (default). For IPV6, use AF_INET6. 
	// To autoselect, use AF_UNSPEC. IPV6 is the newer internet protocol. 
	// Instead of addresses such as natpunch.jenkinssoftware.com, you may have an address
	// such as fe80::7c:31f7:fec4:27de%14. Encoding takes 16 bytes instead of 4, so IPV6
	// is less efficient for bandwidth. NetPeer::startup() will fail if this 
	// IP version is not supported.
	// JINRA_SUPPORT_IPV6 must be set to 1 in NetDefines.h for AF_INET6.
	s16 socketFamily;

	// Set to true to use a blocking socket (default, do not change unless
	// you have a reason to).
	bool blockingSocket;
};

} // namespace Jinra