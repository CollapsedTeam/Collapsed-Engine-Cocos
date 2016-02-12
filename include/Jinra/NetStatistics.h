#pragma once

#include "Packet.h"

namespace Jinra
{

enum RNSPerSecondMetrics
{
	// How many bytes per pushed via a call to NetPeer::send().
	USER_MESSAGE_BYTES_PUSHED,

	// How many user message bytes were sent via a call to NetPeer::send(). 
	// This is less than or equal to USER_MESSAGE_BYTES_PUSHED.
	// A message would be pushed, but not yet sent, due to congestion control.
	USER_MESSAGE_BYTES_SENT,

	// How many user message bytes were resent. A message is resent if it is 
	// marked as reliable, and either the message didn't arrive or the message 
	// ack didn't arrive.
	USER_MESSAGE_BYTES_RESENT,

	// How many user message bytes were received, and returned to the user successfully.
	USER_MESSAGE_BYTES_RECEIVED_PROCESSED,

	// How many user message bytes were received, but ignored due to data format 
	// errors. This will usually be 0.
	USER_MESSAGE_BYTES_RECEIVED_IGNORED,

	// How many actual bytes were sent, including per-message and per-datagram 
	// overhead, and reliable message acks.
	ACTUAL_BYTES_SENT,

	// How many actual bytes were received, including overead and acks.
	ACTUAL_BYTES_RECEIVED,

	RNS_PER_SECOND_METRICS_COUNT
};

struct JINRA_DLL_EXPORT NetStatistics
{
	// For each type in RNSPerSecondMetrics, what is the value over the last 1 second?
	u64 valueOverLastSecond[RNS_PER_SECOND_METRICS_COUNT];

	// For each type in RNSPerSecondMetrics, what is the total value over the lifetime
	// of the connection?
	u64 runningTotal[RNS_PER_SECOND_METRICS_COUNT];
	
	// When did the connection start?
	TimeUS connectionStartTime;

	// Is our current send rate throttled by congestion control?
	// This value should be true if you send more data per second than your bandwidth capacity
	bool isLimitedByCongestionControl;

	// If isLimitedByCongestionControl is true, what is the limit, in bytes per second?
	u64 BPSLimitByCongestionControl;

	// Is our current send rate throttled by a call to 
	// NetPeer::setPerConnectionOutgoingBandwidthLimit()?
	bool isLimitedByOutgoingBandwidthLimit;

	/// If isLimitedByOutgoingBandwidthLimit is true, what is the limit, in bytes per second?
	u64 BPSLimitByOutgoingBandwidthLimit;

	// For each priority level, how many messages are waiting to be sent out?
	u32 messageInSendBuffer[Packet::Priority::NUMBER_OF_PRIORITIES];

	// For each priority level, how many bytes are waiting to be sent out?
	double bytesInSendBuffer[Packet::Priority::NUMBER_OF_PRIORITIES];

	// How many messages are waiting in the resend buffer? This includes messages waiting 
	// for an ack, so should normally be a small value. If the value is rising over time, you 
	// are exceeding the bandwidth capacity.
	u32 messagesInResendBuffer;

	// How many bytes are waiting in the resend buffer. See also messagesInResendBuffer
	u64 bytesInResendBuffer;

	// Over the last second, what was our packetloss? This number will
	// range from 0.0 (for none) to 1.0 (for 100%).
	float packetlossLastSecond;

	// What is the average total packetloss over the lifetime of the connection?
	float packetlossTotal;

	NetStatistics& operator+=(const NetStatistics& netStatistics);
};

/** 
 * Verbosity level currently supports 0 (low), 1 (medium), 2 (high)
 * @param[in] netStatistics The Statistical information to format out.
 * @param[out] buffer The buffer containing a formated report
 * @param[in] verbosityLevel 0 - low | 1 - medium | 2 - high
 */
void JINRA_DLL_EXPORT statisticsToString(NetStatistics* netStatistics, char* buffer, 
										 s32 verbosityLevel);

} // namespace Jinra

