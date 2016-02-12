#pragma once

#include "uint24_t.h"
#include "Packet.h"

namespace Jinra 
{

typedef u32 SplitPacketIndexType;

// This is the counter used for holding packet numbers, so we can detect duplicate packets.  
// It should be large enough that if the variables Internally assumed to 
// be 4 bytes, but written as 3 bytes in ReliabilityLayer::writeToBitStreamFromInternalPacket.
typedef uint24_t MessageNumberType;

// This is the counter used for holding ordered packet numbers, so we can detect out-of-order
// packets. It should be large enough that if the variables were to wrap, the newly wrapped 
// values would no longer be in use. Warning: Too large of a value wastes bandwidth!
typedef MessageNumberType OrderingIndexType;

struct InternalPacketFixedSizeTransmissionHeader
{
	// A unique numerical identifier given to this user message. Used to identify reliable 
	// messages on the network.
	MessageNumberType reliableMessageNumber;

	// The ID used as identification for ordering messages. Also included in sequenced 
	// messages.
	OrderingIndexType orderingIndex;

	// Used only with sequenced messages.
	OrderingIndexType sequencingIndex;

	// What ordering channel this packet is on, if the reliability type uses ordering channels.
	u8 orderingChannel;

	// The ID of the split packet, if we have split packets. This is the maximum number of 
	// split messages we can send simultaneously per connection.
	u16 splitPacketId;

	// If this is a split packet, the index into the array of subsplit packets.
	SplitPacketIndexType splitPacketIndex;

	// The size of the array of subsplit packets.
	SplitPacketIndexType splitPacketCount;

	// How many bits long the data is.
	BitSize_t dataBitLength;

	// What type of reliability algorithm to use with this packet.
	Packet::Reliability reliability;
};

// Holds a user message, and related information.
// Don't use a constructor or destructor, due to the memory pool I am using.
struct InternalPacket : public InternalPacketFixedSizeTransmissionHeader
{
public:
	// Used in InternalPacket when pointing to sharedDataBlock, rather than allocating itself.
	struct RefCountedData
	{
		u8* sharedDataBlock;
		u32 refCount;
	};

public:
	// Identifies the order in which this number was sent. Used locally.
	MessageNumberType messageInternalOrder;

	// Has this message number been assigned yet? We don't assign until the message is 
	// actually sent. This fixes a bug where pre-determining message numbers and then sending 
	// a message on a different channel creates a huge gap. This causes performance problems 
	// and causes those messages to timeout.
	bool messageNumberAssigned;

	// When this packet was created.
	TimeUS creationTime;

	// The resendNext time to take action on this packet.
	TimeUS nextActionTime;

	// For debugging
	TimeUS retransmissionTime;

	// Size of the header when encoded into a bit stream.
	BitSize_t headerLength;

	// Buffer is a pointer to the actual data, assuming this packet has data at all.
	u8* data;

	// How to alloc and delete the data member.
	enum AllocationScheme
	{
		// Data is allocated using malloc. Just free it.
		NORMAL,

		// Data points to a larger block of data, where the larger block is reference counted. 
		// RefCountedData is used in this case.
		REF_COUNTED,
	
		// If allocation scheme is STACK, data points to stackData and should not be 
		// deallocated. This is only used when sending. Received packets are deallocated in 
		// NetPeer.
		STACK
	} allocationScheme;

	RefCountedData* refCountedData;

	// How many attempts we made at sending this message.
	u8 timesSent;

	// The priority level of this packet.
	Packet::Priority priority;

	// If the reliability type requires a receipt, then return this number with it.
	u32 sendReceiptSerial;

	// Used for the resend queue. Linked list implementation so I can remove from the list 
	// via a pointer, without finding it in the list.
	InternalPacket* resendPrev;
	InternalPacket* resendNext; 
	InternalPacket* unreliablePrev;
	InternalPacket* unreliableNext;

	u8 stackData[128];
};

} // namespace Jinra


