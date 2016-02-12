#pragma once

#include "SystemAddress.h"

// For CFSocket
#if defined(__APPLE__)
#import <CoreFoundation/CoreFoundation.h>
#include <netinet/in.h>
#endif

namespace Jinra
{

class RNS2EventHandler;
typedef s32 Socket2;

typedef int RNS2SendResult;

class NetSocket2
{
public:
	enum class Type : u8
	{
		OTHER,
		WINDOWS,
		LINUX
	};

	enum class BindResult : u8
	{
		SUCCESS,
		REQUIRES_JINRA_SUPPORT_IPV6_DEFINE,
		FAILED_TO_BIND_SOCKET,
		FAILED_SEND_TEST,
	};

	struct SendParameters
	{
		SendParameters()
			: ttl(0)
		{ }

		char* data;
		s32 length;
		SystemAddress systemAddress;
		s32 ttl;
	};

	struct RecvStruct
	{
		char data[MAXIMUM_MTU_SIZE];
		s32 bytesRead;
		SystemAddress systemAddress;
		TimeUS timeRead;
		NetSocket2* socket;
	};

public:
	NetSocket2();

	virtual ~NetSocket2() = default;

	virtual RNS2SendResult send(NetSocket2::SendParameters* sendParameters) = 0;
	
	inline bool isBerkleySocket() const {
		return (_socketType != Type::OTHER);
	}

	inline Type getSocketType() const {
		return _socketType;
	}

	inline SystemAddress getBoundAddress() const { 
		return _boundAddress; 
	}

	inline u32 getUserConnectionSocketIndex() const { 
		return _userConnectionSocketIndex; 
	}

	inline RNS2EventHandler* getEventHandler() const { 
		return _eventHandler; 
	}

	inline void setSocketType(Type type) {
		_socketType = type;
	}

	inline void setUserConnectionSocketIndex(u32 index) {
		_userConnectionSocketIndex = index; 
	}

	/** 
	 * In order for the handler to trigger, some platforms must call PollRecvFrom, some 
	 * platforms this create an internal thread.
	 */
	inline void setRecvEventHandler(RNS2EventHandler* eventHandler) {
		_eventHandler = eventHandler;
	}

public:
	static NetSocket2* create();

	static void domainNameToIP(cchar* domainName, char ip[65]);

	static void getMyIP(SystemAddress addresses[MAXIMUM_NUMBER_OF_INTERNAL_IDS]);

protected:
	RNS2EventHandler* _eventHandler;
	Type _socketType;
	SystemAddress _boundAddress;
	u32 _userConnectionSocketIndex;
};

class JINRA_DLL_EXPORT RNS2EventHandler
{
public:
	virtual ~RNS2EventHandler() = default;

	virtual void onRNS2Recv(NetSocket2::RecvStruct* recvStruct) = 0;

	virtual void deallocRNS2RecvStruct(NetSocket2::RecvStruct* socket) = 0;

	virtual NetSocket2::RecvStruct* allocRNS2RecvStruct() = 0;
};

} // namespace Jinra
