#pragma once

#include "NetPeer.h"

namespace Jinra
{

class JINRA_DLL_EXPORT NetClient
{
public:
	NetClient();

	virtual ~NetClient();

	NetPeer::StartupResult initialize(u16 port = 0);

	void connect(cchar* ip, u16 port, cchar* connectionPassword = "Jinra");

	void reconnect(cchar* password);

	void receive();

	void send(const BitStream* bitStream, Packet::Priority priority = Packet::IMMEDIATE_PRIORITY);

	void shutdown(u32 waitTime);

	NetPeer* getPeer() const {
		return _peer;
	}

protected:
	virtual void handlePacket(Packet* packet, BitStream* bitStream) = 0;

protected:
	NetPeer* _peer;
	SystemAddress _connectionAddress;
};

} // namespace Jinra