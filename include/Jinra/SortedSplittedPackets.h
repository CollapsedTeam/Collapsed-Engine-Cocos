#pragma once

#include "InternalPacket.h"

namespace Jinra
{

typedef u16 SplitPacketIdType;

class SortedSplittedPackets
{
public:
	SortedSplittedPackets();

	~SortedSplittedPackets();

	void preallocate(InternalPacket* internalPacket);

	bool add(InternalPacket* internalPacket);

	 u32 getAllocSize() const {
		return _allocationSize;
	}

	 u32 getAddedPacketsCount() const {
		return _addedPacketsCount;
	}

	InternalPacket* get(u32 index);

	SplitPacketIdType getPacketID();

private:
	InternalPacket** _data;
	u32 _allocationSize;
	u32 _addedPacketsCount;
	SplitPacketIdType _packetId;
};

} // namespace Jinra