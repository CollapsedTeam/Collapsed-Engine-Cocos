#pragma once

#include "NetPeer.h"

namespace Jinra
{

class JINRA_DLL_EXPORT NetServer
{
public:
	NetServer();

	virtual ~NetServer();

	NetPeer::StartupResult initialize(u16 port, u32 maxConnections, 
									  cchar* connectionPassword = "Jinra");

	void receive();

	u32 send(const BitStream* bitStream, Packet::Priority priority, const NetGUID& guid);

	u32 sendToAll(const BitStream* bitStream, Packet::Priority priority);

	u32 sendToAllExcept(const BitStream* bitStream, Packet::Priority priority,
						const NetGUID& guid);

	void shutdown(u32 waitTime);

	inline NetPeer* getPeer() const {
		return _peer;
	}

protected:
	virtual void handlePacket(Packet* packet, BitStream* bitStream) = 0;

private:
	NetPeer* _peer;
};

} // namespace Jinra