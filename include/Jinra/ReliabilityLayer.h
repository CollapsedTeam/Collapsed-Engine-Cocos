#pragma once

#include "SplitPacketChannel.h"
#include "BPSTracker.h"
#include "Packet.h"

#include "DS_Queue.h"
#include "DS_LinkedList.h"
#include "BitStream.h"
#include "InternalPacket.h"
#include "NetStatistics.h"
#include "DS_OrderedList.h"
#include "DS_RangeList.h"
#include "DS_MemoryPool.h"
#include "NetDefines.h"
#include "DS_Heap.h"
#include "BitStream.h"
#include "BerkleySocket2.h"

#include "NetSlidingWindow.h"

/// Number of ordered streams available. You can use up to 32 ordered streams
#define NUMBER_OF_ORDERED_STREAMS 32 // 2^5

#define RESEND_TREE_ORDER 32

namespace Jinra
{

typedef u64 reliabilityHeapWeightType;

/// Datagram reliable, ordered, unordered and sequenced sends.  Flow control.  Message splitting, reassembly, and coalescence.
class ReliabilityLayer
{
public:
	// Constructor
	ReliabilityLayer();

	// Destructor
	~ReliabilityLayer();

	/// Resets the layer for reuse
	void reset(bool resetVariables, s32 MTUSize);

	/// Packets are read directly from the socket layer and skip the reliability layer because unconnected players do not use the reliability layer
	/// This function takes packet data after a player has been confirmed as connected.
	/// \param[in] buffer The socket data
	/// \param[in] length The length of the socket data
	/// \param[in] systemAddress The player that this data is from
	/// \param[in] messageHandlerList A list of registered plugins
	/// \param[in] MTUSize maximum datagram size
	/// \retval true Success
	/// \retval false Modified packet
	bool handleSocketReceiveFromConnectedPlayer(cchar* buffer, u32 length,
												SystemAddress &systemAddress, NetSocket2* s,
												TimeUS timeRead, BitStream &updateBitStream);

	/// This allocates bytes and writes a user-level message to those bytes.
	/// \param[out] data The message
	/// \return Returns number of BITS put into the buffer
	BitSize_t receive(u8** data);

	/// Puts data on the send queue
	/// \param[in] data The data to send
	/// \param[in] numberOfBitsToSend The length of \a data in bits
	/// \param[in] priority The priority level for the send
	/// \param[in] reliability The reliability type for the send
	/// \param[in] orderingChannel 0 to 31.  Specifies what channel to use, for relational ordering and sequencing of packets.
	/// \param[in] makeDataCopy If true \a data will be copied.  Otherwise, only a pointer will be stored.
	/// \param[in] MTUSize maximum datagram size
	/// \param[in] currentTime Current time, as per Jinra::GetTimeMS()
	/// \param[in] receipt This number will be returned back with ID_SND_RECEIPT_ACKED or ID_SND_RECEIPT_LOSS and is only returned with the reliability types that contain RECEIPT in the name
	/// \return True or false for success or failure.
	bool send(char* data, BitSize_t numberOfBitsToSend, Packet::Priority priority,
			  Packet::Reliability reliability, u8 orderingChannel, bool makeDataCopy,
			  TimeUS currentTime, u32 receipt);

	/// Call once per game cycle.  Handles internal lists and actually does the send.
	/// \param[in] s the communication  end point
	/// \param[in] systemAddress The Unique Player Identifier who shouldhave sent some packets
	/// \param[in] MTUSize maximum datagram size
	/// \param[in] time current system time
	/// \param[in] maxBitsPerSecond if non-zero, enforces that outgoing bandwidth does not exceed this amount
	/// \param[in] messageHandlerList A list of registered plugins
	void update(NetSocket2* s, SystemAddress &systemAddress, TimeUS time,
				u32 bitsPerSecondLimit, BitStream &updateBitStream);

	/// Causes IsDeadConnection to return true
	void killConnection();

	/// Get Statistics
	/// \return A pointer to a static struct, filled out with current statistical information.
	NetStatistics* getStatistics(NetStatistics* rns);

	// Set outgoing lag and packet loss properties
	void applyNetworkSimulator(double _maxSendBPS, TimeMS _minExtraPing,
							   TimeMS _extraPingVariance);

	/// Has a lot of time passed since the last ack
	bool ackTimeout(Time curTime);

	/// Were you ever unable to deliver a packet despite retries?
	/// \return true means the connection has been lost.  Otherwise not.
	inline bool isDeadConnection() const {
		return deadConnection;
	}

	///Are we waiting for any data to be sent out or be processed by the player?
	inline bool isOutgoingDataWaiting() const {
		return (outgoingPacketBuffer.size() > 0) ?
			true : (_statistics.messagesInResendBuffer != 0);
	}

	inline bool areAcksWaiting() const {
		return _acknowlegements.size() > 0;
	}

	/// Returns the value passed to SetTimeoutTime. or the default if it was never called
	/// \param[out] the value passed to SetTimeoutTime
	inline TimeMS getTimeoutTime() const {
		return _timeoutTime;
	}

	inline TimeUS getNextSendTime() const {
		return _nextSendTime;
	}

	inline TimeUS getTimeBetweenPackets() const {
		return _timeBetweenPackets;
	}

	inline TimeMS getTimeLastDatagramArrived() const {
		return timeLastDatagramArrived;
	}

	/// Set the time, in MS, to use before considering ourselves disconnected after not being able to deliver a reliable packet
	/// Default time is 10,000 or 10 seconds in release and 30,000 or 30 seconds in debug.
	/// \param[in] time Time, in MS
	inline void setTimeoutTime(TimeMS time){
		_timeoutTime = time;
	}

	inline void setSplitMessageProgressInterval(s32 interval) {
		splitMessageProgressInterval = interval;
	}

	inline void setUnreliableTimeout(TimeMS timeoutMS) {
		unreliableTimeout = static_cast<TimeUS>(timeoutMS * static_cast<TimeUS>(1000));
	}

private:
	/// Send the contents of a bitstream to the socket
	/// \param[in] s The socket used for sending data
	/// \param[in] systemAddress The address and port to send to
	/// \param[in] bitStream The data to send.
	void SendBitStream(NetSocket2 *s, SystemAddress &systemAddress, BitStream *bitStream, TimeUS currentTime);

	///Parse an internalPacket and create a bitstream to represent this data
	/// \return Returns number of bits used
	BitSize_t WriteToBitStreamFromInternalPacket(Jinra::BitStream *bitStream, const InternalPacket *const internalPacket, TimeUS curTime);


	/// Parse a bitstream and create an internal packet to represent this data
	InternalPacket* CreateInternalPacketFromBitStream(Jinra::BitStream *bitStream, TimeUS time);

	/// Does what the function name says
	u32 RemovePacketFromResendListAndDeleteOlderReliableSequenced(const MessageNumberType messageNumber);

	/// Acknowledge receipt of the packet with the specified messageNumber
	void SendAcknowledgementPacket(const DatagramSequenceNumberType messageNumber);

	/// Parse an internalPacket and figure out how many header bits would be written.  Returns that number
	BitSize_t GetMaxMessageHeaderLengthBits();
	BitSize_t GetMessageHeaderLengthBits(const InternalPacket *const internalPacket);

	/// Returns true if newPacketOrderingIndex is older than the waitingForPacketOrderingIndex
	bool IsOlderOrderedPacket(OrderingIndexType newPacketOrderingIndex, OrderingIndexType waitingForPacketOrderingIndex);

	/// Split the passed packet into chunks under MTU_SIZE bytes (including headers) and save those new chunks
	void SplitPacket(InternalPacket *internalPacket);

	/// Insert a packet into the split packet list
	void InsertIntoSplitPacketList(InternalPacket * internalPacket, TimeUS time);

	/// Take all split chunks with the specified splitPacketId and try to reconstruct a packet. If we can, allocate and return it.  Otherwise return 0
	InternalPacket * BuildPacketFromSplitPacketList(SplitPacketIdType splitPacketId, TimeUS time,
													NetSocket2 *s, SystemAddress &systemAddress, BitStream &updateBitStream);
	InternalPacket * BuildPacketFromSplitPacketList(SplitPacketChannel *splitPacketChannel, TimeUS time);

	/// Delete any unreliable split packets that have long since expired
	//void DeleteOldUnreliableSplitPackets( TimeUS time );

	/// Creates a copy of the specified internal packet with data copied from the original starting at dataByteOffset for dataByteLength bytes.
	/// Does not copy any split data parameters as that information is always generated does not have any reason to be copied
	InternalPacket * CreateInternalPacketCopy(InternalPacket *original, int dataByteOffset, int dataByteLength, TimeUS time);

	/// Inserts a packet into the resend list in order
	void insertPacketIntoResendList(InternalPacket *internalPacket, bool modifyUnacknowledgedBytes);

	/// Memory handling
	void freeThreadSafeMemory();

	// Initialize the variables
	void initializeVariables();

	// Make it so we don't do resends within a minimum threshold of time
	void UpdateNextActionTime();

	/// How many elements are waiting to be resent?
	inline u32 getResendListDataSize() const {
		return _statistics.messagesInResendBuffer;
	}

	/// Update all memory which is not threadsafe
	void UpdateThreadedMemory();

	void CalculateHistogramAckSize();

	std::deque<InternalPacket*> outputQueue;
	s32 splitMessageProgressInterval;
	TimeUS unreliableTimeout;

	struct MessageNumberNode
	{
		DatagramSequenceNumberType messageNumber;
		MessageNumberNode *next;
	};

	struct DatagramHistoryNode
	{
		DatagramHistoryNode() {}

		DatagramHistoryNode(MessageNumberNode *_head, TimeUS ts)
			: head(_head), timeSent(ts)
		{}

		MessageNumberNode* head;

		TimeUS timeSent;
	};
	// Queue length is programmatically restricted to DATAGRAM_MESSAGE_ID_ARRAY_LENGTH
	// This is essentially an O(1) lookup to get a DatagramHistoryNode given an index
	// datagramHistory holds a linked list of MessageNumberNode. Each MessageNumberNode refers to one element in resendList which can be cleared on an ack.
	std::deque<DatagramHistoryNode> datagramHistory;
	DataStructures::MemoryPool<MessageNumberNode> datagramHistoryMessagePool;

	struct UnreliableWithAckReceiptNode
	{
		UnreliableWithAckReceiptNode() {}
		UnreliableWithAckReceiptNode(DatagramSequenceNumberType _datagramNumber, u32 _sendReceiptSerial, Jinra::TimeUS _nextActionTime) :
			datagramNumber(_datagramNumber), sendReceiptSerial(_sendReceiptSerial), nextActionTime(_nextActionTime)
		{}
		DatagramSequenceNumberType datagramNumber;
		u32 sendReceiptSerial;
		TimeUS nextActionTime;
	};
	std::vector<UnreliableWithAckReceiptNode> unreliableWithAckReceiptHistory;

	void removeFromDatagramHistory(DatagramSequenceNumberType index);
	MessageNumberNode* GetMessageNumberNodeByDatagramIndex(DatagramSequenceNumberType index, TimeUS *timeSent);
	void addFirstToDatagramHistory(TimeUS timeSent);
	MessageNumberNode* addFirstToDatagramHistory(DatagramSequenceNumberType messageNumber, TimeUS timeSent);
	MessageNumberNode* addSubsequentToDatagramHistory(MessageNumberNode *messageNumberNode, DatagramSequenceNumberType messageNumber);
	DatagramSequenceNumberType datagramHistoryPopCount;

	DataStructures::MemoryPool<InternalPacket> internalPacketPool;
	InternalPacket *resendBuffer[RESEND_BUFFER_ARRAY_LENGTH];
	InternalPacket *resendLinkedListHead;
	InternalPacket *unreliableLinkedListHead;
	void removeFromUnreliableLinkedList(InternalPacket *internalPacket);
	void AddToUnreliableLinkedList(InternalPacket *internalPacket);

	TimeMS timeLastDatagramArrived;

	DataStructures::Heap<reliabilityHeapWeightType, InternalPacket*, false> outgoingPacketBuffer;
	reliabilityHeapWeightType outgoingPacketBufferNextWeights[Packet::NUMBER_OF_PRIORITIES];
	void initHeapWeights();
	reliabilityHeapWeightType GetNextWeight(s32 priorityLevel);

	DataStructures::OrderedList<SplitPacketIdType, SplitPacketChannel*, splitPacketChannelComp> splitPacketChannelList;

	MessageNumberType sendReliableMessageNumberIndex;
	MessageNumberType internalOrderIndex;
	bool deadConnection;
	SplitPacketIdType splitPacketId;

	// Wait longer to disconnect in debug so I don't get disconnected while tracing
	TimeMS _timeoutTime;

	NetStatistics _statistics;

	// Algorithm for blending ordered and sequenced on the same channel:
	// 1. Each ordered message transmits OrderingIndexType orderedWriteIndex. There are NUMBER_OF_ORDERED_STREAMS independent values of these. The value
	//    starts at 0. Every time an ordered message is sent, the value increments by 1
	// 2. Each sequenced message contains the current value of orderedWriteIndex for that channel, and additionally OrderingIndexType sequencedWriteIndex. 
	//    sequencedWriteIndex resets to 0 every time orderedWriteIndex increments. It increments by 1 every time a sequenced message is sent.
	// 3. The receiver maintains the next expected value for the orderedWriteIndex, stored in orderedReadIndex.
	// 4. As messages arrive:
	//    If a message has the current ordering index, and is sequenced, and is < the current highest sequence value, discard
	//    If a message has the current ordering index, and is sequenced, and is >= the current highest sequence value, return immediately
	//    If a message has a greater ordering index, and is sequenced or ordered, buffer it
	//    If a message has the current ordering index, and is ordered, buffer, then push off messages from buffer
	// 5. Pushing off messages from buffer:
	//    Messages in buffer are put in a minheap. The value of each node is calculated such that messages are returned:
	//    A. (lowest ordering index, lowest sequence index)
	//    B. (lowest ordering index, no sequence index)
	//    Messages are pushed off until the heap is empty, or the next message to be returned does not preserve the ordered index
	//    For an empty heap, the heap weight should start at the lowest value based on the next expected ordering index, to avoid variable overflow

	// Sender increments this by 1 for every ordered message sent
	OrderingIndexType orderedWriteIndex[NUMBER_OF_ORDERED_STREAMS];
	// Sender increments by 1 for every sequenced message sent. Resets to 0 when an ordered message is sent
	OrderingIndexType sequencedWriteIndex[NUMBER_OF_ORDERED_STREAMS];
	// Next expected index for ordered messages.
	OrderingIndexType orderedReadIndex[NUMBER_OF_ORDERED_STREAMS];
	// Highest value received for sequencedWriteIndex for the current value of orderedReadIndex on the same channel.
	OrderingIndexType highestSequencedReadIndex[NUMBER_OF_ORDERED_STREAMS];
	DataStructures::Heap<reliabilityHeapWeightType, InternalPacket*, false> orderingHeaps[NUMBER_OF_ORDERED_STREAMS];
	OrderingIndexType heapIndexOffsets[NUMBER_OF_ORDERED_STREAMS];

	DataStructures::Queue<bool> _hasReceivedPacketQueue;
	DatagramSequenceNumberType _receivedPacketsBaseIndex;
	bool _resetReceivedPackets;

	TimeUS _lastUpdateTime;
	TimeUS _timeBetweenPackets;
	TimeUS _nextSendTime;

	// Limit changes in throughput to once per ping - otherwise even if lag starts we don't know about it
	// In the meantime the connection is flooded and overrun.
	bool _bandwidthExceededStatistic;

	u32 _receivePacketCount;

#if defined(JINRA_DEBUG)
	struct DataAndTime
	{
		NetSocket2 *s;
		char data[MAXIMUM_MTU_SIZE];
		u32 length;
		TimeMS sendTime;
		u32 extraSocketOptions;
	};
	std::deque<DataAndTime*> delayList;

	double _packetloss;
	TimeMS _minExtraPing;
	TimeMS _extraPingVariance;
#endif // defined(JINRA_DEBUG)

	NetSlidingWindow _congestionManager;


	u32 _unacknowledgedBytes;

	bool ResendBufferOverflow() const;
	void ResetPacketsAndDatagrams();
	void PushPacket(InternalPacket *internalPacket, bool isReliable);
	void PushDatagram();
	void clearPacketsAndDatagrams();
	void removeFromList(InternalPacket *internalPacket, bool modifyUnacknowledgedBytes);
	void addToListTail(InternalPacket *internalPacket, bool modifyUnacknowledgedBytes);
	void popListHead(bool modifyUnacknowledgedBytes);
	inline bool isResendQueueEmpty() const {
		return (resendLinkedListHead == nullptr);
	}
	void SendACKs(NetSocket2 *s, SystemAddress &systemAddress, TimeUS time, BitStream &updateBitStream);

	std::vector<InternalPacket*> _packetsToSendThisUpdate;
	std::vector<bool> _packetsToDeallocThisUpdate;
	// boundary is in packetsToSendThisUpdate, inclusive
	std::vector<u32> _packetsToSendThisUpdateDatagramBoundaries;
	std::vector<bool> _datagramsToSendThisUpdateIsPair;
	std::vector<u32> _datagramSizesInBytes;
	BitSize_t _datagramSizeSoFar;
	BitSize_t _allDatagramSizesSoFar;
	double _totalUserDataBytesAcked;
	TimeUS _timeOfLastContinualSend;
	TimeUS _timeToNextUnreliableCull;

	// This doesn't need to be a member, but I do it to avoid reallocations
	DataStructures::RangeList<DatagramSequenceNumberType> _incomingAcks;

	// Every 16 datagrams, we make sure the 17th datagram goes out the same update tick, and is the same size as the 16th
	s32 _countdownToNextPacketPair;
	InternalPacket* allocateFromInternalPacketPool();
	void releaseToInternalPacketPool(InternalPacket *ip);

	DataStructures::RangeList<DatagramSequenceNumberType> _acknowlegements;
	DataStructures::RangeList<DatagramSequenceNumberType> _NAKs;
	bool _remoteSystemNeedsBAndAS;

	u32 getMaxDatagramSizeExcludingMessageHeaderBytes() const;

	inline BitSize_t GetMaxDatagramSizeExcludingMessageHeaderBits() const {
		return BYTES_TO_BITS(getMaxDatagramSizeExcludingMessageHeaderBytes());
	}

	// ourOffset refers to a section within externallyAllocatedPtr. Do not deallocate externallyAllocatedPtr until all references are lost
	void allocInternalPacketData(InternalPacket* internalPacket,
								 InternalPacket::RefCountedData** refCounter,
								 u8* externallyAllocatedPtr, u8* ourOffset);

	// Set the data pointer to externallyAllocatedPtr, do not allocate
	void allocInternalPacketData(InternalPacket* internalPacket, u8* externallyAllocatedPtr);

	// Allocate new
	void allocInternalPacketData(InternalPacket* internalPacket, u32 numBytes, bool allowStack);
	void freeInternalPacketData(InternalPacket* internalPacket);
	DataStructures::MemoryPool<InternalPacket::RefCountedData> _refCountedDataPool;

	BPSTracker _bpsMetrics[RNS_PER_SECOND_METRICS_COUNT];
	TimeUS _lastBpsClear;
};

} // namespace Jinra
