#pragma once

#include "SystemAddress.h"
#include "NetGUID.h"

namespace Jinra
{

struct Packet
{
public:
	// These enumerations are used to describe when packets are delivered.
	enum Priority
	{
		// The highest possible priority. These message trigger sends immediately, and are
		// generally not buffered or aggregated into a single datagram.
		IMMEDIATE_PRIORITY,

		// For every 2 IMMEDIATE_PRIORITY messages, 1 HIGH_PRIORITY will be sent.
		// Messages at this priority and lower are buffered to be sent in groups 
		// at 10 millisecond intervals to reduce UDP overhead and better measure 
		// congestion control. 
		HIGH_PRIORITY,

		// For every 2 HIGH_PRIORITY messages, 1 MEDIUM_PRIORITY will be sent.
		// Messages at this priority and lower are buffered to be sent in groups 
		// at 10 millisecond intervals to reduce UDP overhead and better measure 
		// congestion control. 
		MEDIUM_PRIORITY,

		// For every 2 MEDIUM_PRIORITY messages, 1 LOW_PRIORITY will be sent.
		// Messages at this priority and lower are buffered to be sent in groups 
		// at 10 millisecond intervals to reduce UDP overhead and better measure 
		// congestion control. 
		LOW_PRIORITY,

		NUMBER_OF_PRIORITIES
	};

	// These enumerations are used to describe how packets are delivered.
	enum Reliability
	{
		// Same as regular UDP, except that it will also discard duplicate datagrams. 
		// Jinra adds (6 to 17) + 21 bits of overhead, 16 of which is used to detect 
		// duplicate packets and 6 to 17 of which is used for message length.
		UNRELIABLE,

		// Regular UDP with a sequence counter. Out of order messages will be discarded.
		// Sequenced and ordered messages sent on the same channel will arrive in the order sent.
		UNRELIABLE_SEQUENCED,

		// The message is sent reliably, but not necessarily in any order. 
		// Same overhead as UNRELIABLE.
		RELIABLE,

		// This message is reliable and will arrive in the order you sent it.  
		// Messages will be delayed while waiting for out of order messages.  
		// Same overhead as UNRELIABLE_SEQUENCED.
		// Sequenced and ordered messages sent on the same channel will arrive in the order sent.
		RELIABLE_ORDERED,

		// This message is reliable and will arrive in the sequence you sent it. 
		// Out or order messages will be dropped. Same overhead as UNRELIABLE_SEQUENCED.
		// Sequenced and ordered messages sent on the same channel will arrive in the order sent.
		RELIABLE_SEQUENCED,

		// Same as UNRELIABLE, however the user will get either ID_SND_RECEIPT_ACKED or 
		// ID_SND_RECEIPT_LOSS based on the result of sending this message when 
		// calling NetPeer::Receive(). Bytes 1-4 will contain the number returned 
		// from the Send() function. On disconnect or shutdown, all messages not previously 
		// acked should be considered lost.
		UNRELIABLE_WITH_ACK_RECEIPT,

		// Same as RELIABLE. The user will also get ID_SND_RECEIPT_ACKED after the message is 
		// delivered when calling NetPeer::Receive(). ID_SND_RECEIPT_ACKED is returned
		// when the message arrives, not necessarily the order when it was sent. Bytes 1-4 will
		// contain the number returned from the Send() function. On disconnect or shutdown, all
		// messages not previously acked should be considered lost. This does not return 
		// ID_SND_RECEIPT_LOSS.
		RELIABLE_WITH_ACK_RECEIPT,

		// Same as RELIABLE_ORDERED_ACK_RECEIPT. The user will also get ID_SND_RECEIPT_ACKED 
		// after the message is delivered when calling NetPeer::Receive(). 
		// ID_SND_RECEIPT_ACKED is returned when the message arrives, not necessarily 
		// the order when it was sent. Bytes 1-4 will contain the number returned 
		// from the Send() function. On disconnect or shutdown, all messages not previously 
		// acked should be considered lost. This does not return ID_SND_RECEIPT_LOSS.
		RELIABLE_ORDERED_WITH_ACK_RECEIPT,

		NUMBER_OF_RELIABILITIES
	};

public:
	// The system that send this packet.
	SystemAddress systemAddress;

	// A unique identifier for the system that sent this packet, regardless of IP 
	// address (internal / external / remote system).
	// Only valid once a connection has been established 
	// (ID_CONNECTION_REQUEST_ACCEPTED, or ID_NEW_INCOMING_CONNECTION).
	// Until that time, will be UNASSIGNED_NET_GUID.
	NetGUID guid;

	// The length of the data in bytes.
	u32 length;

	// The length of the data in bits.
	BitSize_t bitSize;

	// The data from the sender.
	u8* data;

	// Indicates whether to delete the data, or to simply delete the packet.
	bool deleteData;
};

} // namespace Jinra