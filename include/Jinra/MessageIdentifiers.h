#pragma once

/** 
 * You should not edit this file as it is a part of Jinra library.
 * To define your own packet id, define an enum following the code example that follows. 
 *
 * enum {
 *   ID_PACKET_1 = ID_USER_PACKET_ENUM,
 *   ID_PACKET_2, 
 *    ... 
 * };
 *
 * All these enumerations should be casted to u8(u8) before writing them to BitStream.
 */
enum DefaultMessageTypes
{
	// what(type : bytes)

	// Internal use only
	// Ping from a connected system. Update timestamps.
	// id(u8 : 1) + time(Time : 4 or 8)
	ID_CONNECTED_PING,  

	// Internal use only.
	// Ping from an unconnected system. Reply but do not update timestamps.
	// id(u8 : 1) + time(Time : 4 or 8) + ...
	ID_UNCONNECTED_PING,

	// Ping from an unconnected system. Only reply if we have open connections. 
	// Do not update timestamps (internal use only).
	ID_UNCONNECTED_PING_OPEN_CONNECTIONS,

	// Pong from a connected system. Update timestamps (internal use only).
	ID_CONNECTED_PONG,

	// A reliable packet to detect lost connections (internal use only).
	ID_DETECT_LOST_CONNECTIONS,

	// C2S: Initial query: Header(1), OfflineMesageID(16), Protocol number(1), Pad(toMTU), 
	// sent with no fragment set.
	// If protocol fails on server, returns ID_INCOMPATIBLE_PROTOCOL_VERSION to client.
	ID_OPEN_CONNECTION_REQUEST_1,

	// S2C: Header(1), OfflineMesageID(16), server GUID(8), HasSecurity(1), 
	// Cookie(4, if HasSecurity), public key (if do security is true), MTU(2). 
	// If public key fails on client, returns ID_PUBLIC_KEY_MISMATCH.
	ID_OPEN_CONNECTION_REPLY_1,

	// C2S: Header(1), OfflineMesageID(16), Cookie(4, if HasSecurity is true on the server), 
	// clientSupportsSecurity(1 bit), remoteBindingAddress(6), MTU(2), client GUID(8)
	// Connection slot allocated if cookie is valid, server is not full, GUID and IP 
	// not already in use.
	ID_OPEN_CONNECTION_REQUEST_2,

	// S2C: Header(1), OfflineMesageID(16), server GUID(8), mtu(2), doSecurity(1 bit)
	ID_OPEN_CONNECTION_REPLY_2,

	// C2S: Header(1), GUID(8), Timestamp, Proof(32)
	ID_CONNECTION_REQUEST,

	// NetPeer - Wrong public key passed to NetPeer::Connect()
	ID_PUBLIC_KEY_MISMATCH,

	// NetPeer - Same as ID_ADVERTISE_SYSTEM, but intended for internal use rather than
	// being passed to the user.
	// Second byte indicates type. Used currently for NAT punchthrough for receiver port
	// advertisement. See ID_NAT_ADVERTISE_RECIPIENT_PORT
	ID_OUT_OF_BAND_INTERNAL,

	// If NetPeer::Send() is called where PacketReliability contains
	// _WITH_ACK_RECEIPT, then on a later call to NetPeer::Receive() you will get 
	// ID_SND_RECEIPT_ACKED or ID_SND_RECEIPT_LOSS. The message will be 5 bytes long,
	// and bytes 1-4 inclusive will contain a number in native order containing a number that
	// identifies this message. This number will be returned by NetPeer::Send() 
	// or NetPeer::SendList(). ID_SND_RECEIPT_ACKED means that the message arrived
	ID_SND_RECEIPT_ACKED,

	// If NetPeer::Send() is called where PacketReliability contains 
	// UNRELIABLE_WITH_ACK_RECEIPT, then on a later call to NetPeer::Receive() you
	// will get ID_SND_RECEIPT_ACKED or ID_SND_RECEIPT_LOSS. The message will be 5 bytes long,
	// and bytes 1-4 inclusive will contain a number in native order containing a number that
	// identifies this message. This number will be returned by NetPeer::Send() or 
	// NetPeer::SendList(). ID_SND_RECEIPT_LOSS means that an ack for the message did
	// not arrive (it may or may not have been delivered, probably not). On disconnect
	// or shutdown, you will not get ID_SND_RECEIPT_LOSS for unsent messages, you should
	// consider those messages as all lost.
	ID_SND_RECEIPT_LOSS,
	
	// NetPeer - In a client/server environment, our connection request to the server has 
	// been accepted.
	ID_CONNECTION_REQUEST_ACCEPTED,

	// NetPeer - Sent to the player when a connection request cannot be completed due to
	// inability to connect. 
	ID_CONNECTION_ATTEMPT_FAILED,

	// NetPeer - Sent a connect request to a system we are currently connected to.
	ID_ALREADY_CONNECTED,

	// NetPeer - A remote system has successfully connected.
	ID_NEW_INCOMING_CONNECTION,

	// NetPeer - The system we attempted to connect to is not accepting new connections.
	ID_NO_FREE_INCOMING_CONNECTIONS,

	// NetPeer - The system specified in Packet::systemAddress has disconnected from us. 
	// For the client, this would mean the server has shutdown. 
	ID_DISCONNECTION_NOTIFICATION,

	// NetPeer - Reliable packets cannot be delivered to the system specified in 
	// Packet::systemAddress. The connection to that system has been closed. 
	ID_CONNECTION_LOST,

	// NetPeer - We are banned from the system we attempted to connect to.
	ID_CONNECTION_BANNED,

	// NetPeer - The remote system is using a password and has refused our connection 
	// because we did not set the correct password.
	ID_INVALID_PASSWORD,

	// JINRA_PROTOCOL_VERSION does not match on the remote system what we have on our system
	// This means the two systems cannot communicate. The 2nd byte of the message contains 
	// the value of JINRA_PROTOCOL_VERSION for the remote system.
	ID_INCOMPATIBLE_PROTOCOL_VERSION,

	// Means that this IP address connected recently, and can't connect again as a security
	// measure. See NetPeer::setLimitIPConnectionFrequency().
	ID_IP_RECENTLY_CONNECTED,

	// NetPeer - The sizeof(Time) bytes following this byte represent a value which is 
	// automatically modified by the difference in system times between the sender and the 
	// recipient. Requires that you call setOccasionalPing.
	ID_TIMESTAMP,

    // NetPeer - Pong from an unconnected system. First byte is ID_UNCONNECTED_PONG, second 
	// sizeof(TimeMS) bytes is the ping, following bytes is system specific enumeration data.
	// Read using bitstreams
	ID_UNCONNECTED_PONG,

	// NetPeer - Inform a remote system of our IP/Port. On the recipient, all data past 
	// ID_ADVERTISE_SYSTEM is whatever was passed to the data parameter.
	ID_ADVERTISE_SYSTEM,

	// NetPeer - Downloading a large message. Format is ID_DOWNLOAD_PROGRESS (MessageID), 
	// partCount (u32), partTotal (u32), partLength (u32), first part data 
	// (length <= MAX_MTU_SIZE). See the three parameters partCount, partTotal and partLength
	// in OnFileProgress in FileListTransferCBInterface.h
	ID_DOWNLOAD_PROGRESS,

	ID_RESERVED_1,
	ID_RESERVED_2,
	ID_RESERVED_3,
	ID_RESERVED_4,
	ID_RESERVED_5,
	ID_RESERVED_6,
	ID_RESERVED_7,
	ID_RESERVED_8,
	ID_RESERVED_9,
	ID_RESERVED_10,
	ID_RESERVED_11,
	ID_RESERVED_12,
	ID_RESERVED_13,
	ID_RESERVED_14,
	ID_RESERVED_15,
	ID_RESERVED_16,
	ID_RESERVED_17,
	ID_RESERVED_18,
	ID_RESERVED_19,
	ID_RESERVED_20,

	// For the user to use. Start your first enumeration at this value.
	ID_USER_PACKET_ENUM 
};
