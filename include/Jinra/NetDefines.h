#pragma once

// Maximum (stack) size to use with _alloca before using new and delete instead.
#ifndef MAX_ALLOCA_STACK_ALLOCATION
#define MAX_ALLOCA_STACK_ALLOCATION 1048576
#endif

// Threshold at which to do a malloc / free rather than pushing data onto a fixed stack for 
// the bit stream class. Arbitrary size, just picking something likely to be larger than most 
// packets.
#ifndef BITSTREAM_STACK_ALLOCATION_SIZE
#define BITSTREAM_STACK_ALLOCATION_SIZE 256
#endif

// Maximum number of local IP addresses supported
#ifndef MAXIMUM_NUMBER_OF_INTERNAL_IDS
#define MAXIMUM_NUMBER_OF_INTERNAL_IDS 10
#endif

// This controls the amount of memory used per connection.
// This many datagrams are tracked by datagramNumber. If more than this many datagrams are 
// sent, then an ack for an older datagram would be ignored. This results in an unnecessary 
// resend in that case.
#ifndef DATAGRAM_MESSAGE_ID_ARRAY_LENGTH
#define DATAGRAM_MESSAGE_ID_ARRAY_LENGTH 512
#endif

// This is the maximum number of reliable user messages that can be on the wire at a time
// If this is too low, then high ping connections with a large throughput will be underutilized
// This will be evident because JinraStatistics::messagesInSend buffer will increase over 
// time, yet at the same time the outgoing bandwidth per second is less than your connection 
// supports.
#ifndef RESEND_BUFFER_ARRAY_LENGTH
#define RESEND_BUFFER_ARRAY_LENGTH 512
#define RESEND_BUFFER_ARRAY_MASK 511
#endif

// Warning: Code for IPV6 needs refactoring and tests.
#ifndef JINRA_SUPPORT_IPV6
//#define JINRA_SUPPORT_IPV6
#endif

// Controls how many allocations occur at once for the memory pool of incoming datagrams waiting 
// to be transferred between the recvfrom thread and the main update thread. Has large effect on
// memory usage, per instance of NetPeer. Approximately 
// MAXIMUM_MTU_SIZE * BUFFERED_PACKETS_PAGE_SIZE bytes, once after calling NetPeer::startup()
#ifndef BUFFERED_PACKETS_PAGE_SIZE
#define BUFFERED_PACKETS_PAGE_SIZE 8
#endif

// Controls how many allocations occur at once for the memory pool of incoming or outgoing 
// datagrams. Has small effect on memory usage per connection. Uses about 
// 256 bytes * INTERNAL_PACKET_PAGE_SIZE per connection.
#ifndef INTERNAL_PACKET_PAGE_SIZE
#define INTERNAL_PACKET_PAGE_SIZE 8
#endif

#ifndef USE_ALLOCA
#define USE_ALLOCA
#endif
