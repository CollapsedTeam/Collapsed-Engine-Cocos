/*
http://www.ssfnet.org/Exchange/tcp/tcpTutorialNotes.html

cwnd=max bytes allowed on wire at once

Start:
cwnd=mtu
ssthresh=unlimited

Slow start:
On ack cwnd*=2

congestion avoidance:
On ack during new period
cwnd+=mtu*mtu/cwnd

on loss or duplicate ack during period:
sshtresh=cwnd/2
cwnd=MTU
This reenters slow start

If cwnd < ssthresh, then use slow start
else use congestion avoidance
*/

#pragma once

#include "uint24_t.h"

// Sizeof an UDP header in byte
#define UDP_HEADER_SIZE 28

namespace Jinra
{

typedef uint24_t DatagramSequenceNumberType;

class NetSlidingWindow
{
public:
	/** 
	 * Reset all variables to their initial states, for a new connection
	 */
	void init(u32 maxDatagramPayload);

	/** 
	 * Acks do not have to be sent immediately. Instead, they can be buffered up such that 
	 * groups of acks are sent at a time.
	 * This reduces overall bandwidth usage.
	 * How long they can be buffered depends on the retransmit time of the sender.
	 * Should call once per update tick, and send if needed.
	 */
	bool shouldSendACKs(TimeUS curTime);

	/** 
	 * Every data packet sent must contain a sequence number
	 * Call this function to get it. The sequence number is passed into OnGotPacketPair()
	 */
	DatagramSequenceNumberType getAndIncrementNextDatagramSequenceNumber();

	inline DatagramSequenceNumberType getNextDatagramSequenceNumber() const {
		return _nextDatagramSequenceNumber;
	}

	/** 
	 * Call this when you get a packet (including packet pairs).
	 * If the DatagramSequenceNumberType is out of order, skippedMessageCount will be non-zero.
	 * In that case, send a NAK for every sequence number up to that count.
	 */
	bool onGotPacket(DatagramSequenceNumberType datagramSequenceNumber, 
					 TimeUS curTime, u32* skippedMessageCount);

	/** 
	 * Call when you get a NAK, with the sequence number of the lost message.
	 * Affects the congestion control.
	 */
	void onResend();
	
	void onNAK();

	/** 
	 * Call this when an ACK arrives.
	 * hasBAndAS are possibly written with the ack, see onSendAck()
	 */
	void onAck(TimeUS rtt, bool isContinuousSend, DatagramSequenceNumberType sequenceNumber);

	/** 
	 * Call when you send an ack, to see if the ack should have the B and AS parameters 
	 * transmitted. 
	 * Call before calling onSendAck()
	 */
	void onSendAckGetBAndAS(bool* hasBAndAS);

	/** 
	 * Call when we send an ack, to write B and AS if needed
	 * B and AS are only written once per SYN, to prevent slow calculations.
	 * Also updates SND, the period between sends, since data is written out.
	 * Be sure to call onSendAckGetBAndAS() before calling onSendAck(), since whether 
	 * you write it or not affects numBytes.
	 */
	void onSendAck();

	inline bool isInSlowStart() const {
		return (_cwnd <= _ssThresh || _ssThresh == 0.0);
	}

	/** 
	 * Retransmission time out for the sender.
	 * If the time difference between when a message was last transmitted, and the current 
	 * time is greater than RTO then packet is eligible for retransmission, pending 
	 * congestion control.
	 * RTO = (RTT + 4 * RTTVar) + SYN
	 * If we have been continuously sending for the last RTO, and no ACK or NAK at all, SND *= 2;
	 * This is per message, which is different from UDT, but Jinra supports packetloss with 
	 * continuing data where UDT is only RELIABLE_ORDERED.
	 * Minimum value is 100 milliseconds.
	 */
	TimeUS getRTOForRetransmission() const;

	/** 
	 * Return what was set by setMTU()
	 */
	inline u32 getMTU() const {
		return _maxMTUIncludingUPDHeader;
	}

	/** 
	 * Query for statistics
	 */
	inline double getRTT() const {
		return (_lastRtt == _UNSET_TIME_US) ? 0.0 : _lastRtt;
	}

	inline s32 getRetransmissionBandwidth(u32 unacknowledgedBytes) const {
		return unacknowledgedBytes;
	}
	
	s32 getTransmissionBandwidth(u32 unacknowledgedBytes, bool isContinuousSend);

	// Set the maximum amount of data that can be sent in one datagram
	// Default to MAXIMUM_MTU_SIZE - UDP_HEADER_SIZE
	void setMTU(u32 bytes);

public:
	/** 
	 * Is a > b, accounting for variable overflow?
	 */
	static bool greaterThan(DatagramSequenceNumberType a, DatagramSequenceNumberType b);
	
	/** 
	 * Is a < b, accounting for variable overflow?
	 */
	static bool lessThan(DatagramSequenceNumberType a, DatagramSequenceNumberType b);

protected:
	TimeUS getSenderRTOForACK() const;

protected:
	// Maximum amount of bytes that the user can send, e.g. the size of one full datagram
	u32 _maxMTUIncludingUPDHeader;

	double _cwnd; // max bytes on wire
	double _ssThresh; // Threshhold between slow start and congestion avoidance

	// When we get an ack, if oldestUnsentAck==0, set it to the current time
	// When we send out acks, set oldestUnsentAck to 0
	TimeUS _oldestUnsentAck;

	// Every outgoing datagram is assigned a sequence number, which increments by 1 every assignment
	DatagramSequenceNumberType _nextDatagramSequenceNumber;
	DatagramSequenceNumberType _nextCongestionControlBlock;
	bool _backoffThisBlock;
	bool _speedUpThisBlock;

	// Track which datagram sequence numbers have arrived.
	// If a sequence number is skipped, send a NAK for all skipped messages
	DatagramSequenceNumberType _expectedNextSequenceNumber;

	bool _isContinuousSend;

	double _lastRtt;
	double _estimatedRTT;
	double _deviationRtt;

	static const double _UNSET_TIME_US;
};

} // namespace Jinra

