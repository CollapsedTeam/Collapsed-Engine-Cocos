#pragma once

#include "NetSocket2.h"
#include "NetThread.h"

namespace Jinra
{

class BerkleySocket2 : public NetSocket2
{
	friend class NetSocket2;

public:
	struct BindParameters
	{
		// Input parameters
		u16 port;
		char* hostAddress;
		u16 addressFamily; // AF_INET or AF_INET6
		s32 type; // SOCK_DGRAM
		s32 protocol; // 0
		bool nonBlockingSocket;
		s32 setBroadcast;
		s32 setIPHdrIncl;
		s32 doNotFragment;
		s32 pollingThreadPriority;
		RNS2EventHandler* eventHandler;
	};

public:
	BerkleySocket2();

	virtual ~BerkleySocket2();

	virtual NetSocket2::BindResult bind(BerkleySocket2::BindParameters* bindParameters) = 0;

	s32 createRecvPollingThread(s32 threadPriority);

	void signalStopRecvPollingThread();

	void blockOnStopRecvPollingThread();

	/**
	 * For addressFamily, use AF_INET
	 * For type, use SOCK_DGRAM
	 */
	static bool isPortInUse(u16 port, cchar* hostAddress, u16 addressFamily, s32 type);

	inline const BerkleySocket2::BindParameters* getBindings() const {
		return &_binding;
	}

	inline Socket2 getSocket() const {
		return _rns2Socket;
	}

	void setDoNotFragment(s32 opt);

protected:
	NetSocket2::BindResult bindShared(BerkleySocket2::BindParameters* bindParameters);
	
	void recvFromBlocking(NetSocket2::RecvStruct* recvFromStruct);
	
	static JINRA_THREAD_DECLARATION(recvFromLoop);

	u32 recvFromLoopInt();

	void setSocketOptions();

	void setNonBlockingSocket(unsigned long nonBlocking);

	void setBroadcastSocket(s32 broadcast);

	void setIPHdrIncl(s32 ipHdrIncl);

private:
	NetSocket2::BindResult bindSharedIPV4(BerkleySocket2::BindParameters* bindParameters);

	NetSocket2::BindResult bindSharedIPV4And6(BerkleySocket2::BindParameters* bindParameters);

	void recvFromBlockingIPV4(NetSocket2::RecvStruct* recvFromStruct);

	void recvFromBlockingIPV4And6(NetSocket2::RecvStruct* recvFromStruct);

	static void domainNameToIP(cchar* domainName, char ip[65]);

	static void getSystemAddressIPV4(Socket2 rns2Socket, SystemAddress* systemAddressOut);

	static void getSystemAddressIPV4And6(Socket2 rns2Socket, SystemAddress* systemAddressOut);

protected:
	Socket2 _rns2Socket;
	BerkleySocket2::BindParameters _binding;

	std::atomic<u32> _isRecvFromLoopThreadActive;
	volatile bool _endThreads;
};

} // namespace Jinra