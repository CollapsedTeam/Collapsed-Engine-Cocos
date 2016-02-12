#pragma once

#include "SortedSplittedPackets.h"

namespace Jinra
{

struct SplitPacketChannel
{
	TimeUS lastUpdateTime;
	SortedSplittedPackets splitPacketList;

	// This is here for progress notifications, since progress notifications return 
	// the first packet data, if available.
	InternalPacket* firstPacket;

};

s32 JINRA_DLL_EXPORT splitPacketChannelComp(SplitPacketIdType const& key, 
											SplitPacketChannel* const& data);

} // namespace Jinra