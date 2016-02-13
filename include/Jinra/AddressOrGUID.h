#pragma once

#include "SystemAddress.h"
#include "NetGUID.h"

namespace Jinra
{

struct Packet;

struct JINRA_DLL_EXPORT AddressOrGUID
{
public:
	AddressOrGUID() = default;

	AddressOrGUID(const AddressOrGUID& input);

	AddressOrGUID(const SystemAddress& input);

	AddressOrGUID(Packet* packet);

	AddressOrGUID(const NetGUID& input);
	
	cchar* toString(bool writePort = true) const;

	void toString(bool writePort, char* dest) const;

	static unsigned long toInteger(const AddressOrGUID& aog);

	 bool isUndefined() const { 
		return (guid == UNASSIGNED_NET_GUID && address == UNASSIGNED_SYSTEM_ADDRESS); 
	}

	 SystemIndex getSystemIndex() const { 
		return (guid != UNASSIGNED_NET_GUID) ? guid.systemIndex : address.systemIndex;
	}

	void setUndefined();

	AddressOrGUID& operator=(const AddressOrGUID& input);

	AddressOrGUID& operator=(const SystemAddress& input);

	AddressOrGUID& operator=(const NetGUID& input);

	 bool operator==(const AddressOrGUID& right) const { 
		return ((guid != UNASSIGNED_NET_GUID && guid == right.guid) || 
				(address != UNASSIGNED_SYSTEM_ADDRESS && address == right.address)); 
	}

public:
	SystemAddress address;
	NetGUID guid;
};

} // namespace Jinra