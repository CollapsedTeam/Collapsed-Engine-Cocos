#pragma once

#include "SocketDescriptor.h"
#include "Packet.h"

#include "ReliabilityLayer.h"
#include "BitStream.h"
#include "DS_OrderedList.h"

#include "NetThread.h"
#include "DS_ThreadsafeAllocatingQueue.h"
#include "SignaledEvent.h"

namespace Jinra 
{

/// Forward declarations
class HuffmanEncodingTree;

const int PING_TIMES_ARRAY_SIZE = 5;

// Sucks but this struct has to be outside the class.  Inside and DevCPP won't let you refer to the struct as NetPeer::RemoteSystemIndex while GCC
// forces you to do NetPeer::RemoteSystemIndex
struct RemoteSystemIndex{unsigned index; RemoteSystemIndex *next;};

///\brief Main interface for network communications.
/// \details It implements most of Jinra's functionality and is the primary interface for Jinra.
///
/// Inherits NetPeer.
///
/// See the individual functions for what the class can do.
/// 
class JINRA_DLL_EXPORT NetPeer : public RNS2EventHandler
{
public:
	enum class StartupResult : u8
	{
		STARTED,
		ALREADY_STARTED,
		INVALID_SOCKET_DESCRIPTORS,
		INVALID_MAX_CONNECTIONS,
		SOCKET_FAMILY_NOT_SUPPORTED,
		SOCKET_PORT_ALREADY_IN_USE,
		SOCKET_FAILED_TO_BIND,
		SOCKET_FAILED_TEST_SEND,
		PORT_CANNOT_BE_ZERO,
		FAILED_TO_CREATE_NETWORK_THREAD,
		COULD_NOT_GENERATE_GUID,
		OTHER_FAILURE
	};

	enum class ConnectionAttemptResult : u8
	{
		STARTED,
		INVALID_PARAMETER,
		CANNOT_RESOLVE_DOMAIN_NAME,
		ALREADY_CONNECTED_TO_ENDPOINT,
		ALREADY_IN_PROGRESS,
		SECURITY_INITIALIZATION_FAILED
	};

	enum class ConnectionState : u8
	{
		// connect() was called, but the process hasn't started yet.
		IS_PENDING,
		// Processing the connection attempt.
		IS_CONNECTING,
		// Is connected and able to communicate.
		IS_CONNECTED,
		// Was connected, but will disconnect as soon as the remaining messages are delivered.
		IS_DISCONNECTING,
		// A connection attempt failed and will be aborted.
		IS_SILENTLY_DISCONNECTING,
		// No longer connected.
		IS_DISCONNECTED,
		// Was never connected, or else was disconnected long enough ago that the entry has 
		// been discarded.
		IS_NOT_CONNECTED
	};

public:	
	///Constructor
	NetPeer();

	///Destructor
	virtual ~NetPeer();

	// --------------------------------------------------------------------------------------------Major Low Level Functions - Functions needed by most users--------------------------------------------------------------------------------------------
	/// \brief Starts the network threads and opens the listen port.
	/// \details You must call this before calling Connect().
	/// \note Multiple calls while already active are ignored.  To call this function again with different settings, you must first call Shutdown().
	/// \note Call SetMaximumIncomingConnections if you want to accept incoming connections.
	/// \param[in] maxConnections Maximum number of connections between this instance of NetPeer and another instance of NetPeer. Required so that the network can preallocate and for thread safety. A pure client would set this to 1.  A pure server would set it to the number of allowed clients.A hybrid would set it to the sum of both types of connections.
	/// \param[in] localPort The port to listen for connections on. On linux the system may be set up so thast ports under 1024 are restricted for everything but the root user. Use a higher port for maximum compatibility. 
	/// \param[in] socketDescriptors An array of SocketDescriptor structures to force Jinra to listen on a particular IP address or port (or both).  Each SocketDescriptor will represent one unique socket.  Do not pass redundant structures.  To listen on a specific port, you can pass SocketDescriptor(myPort,0); such as for a server.  For a client, it is usually OK to just pass SocketDescriptor(); 
	/// \param[in] socketDescriptorCount The size of the \a socketDescriptors array.  Pass 1 if you are not sure what to pass.
	/// \param[in] threadPriority Passed to the thread creation routine. Use THREAD_PRIORITY_NORMAL for Windows. For Linux based systems, you MUST pass something reasonable based on the thread priorities for your application.
	/// \return JINRA_STARTED on success, otherwise appropriate failure enumeration.
	StartupResult startup(u32 maxConnections, SocketDescriptor* socketDescriptors, 
						  u32 socketDescriptorCount, s32 threadPriority = -99999 );

	/// \brief Sets the maximum number of incoming connections allowed.
	/// \details If the number of incoming connections is less than the number of players currently connected,
	/// no more players will be allowed to connect.  If this is greater than the maximum number of peers allowed,
	/// it will be reduced to the maximum number of peers allowed.
	/// 
	/// Defaults to 0, meaning by default, nobody can connect to you
	/// \param[in] numberAllowed Maximum number of incoming connections allowed.
	 void setMaximumIncomingConnections(u32 numberAllowed) {
		maximumIncomingConnections = numberAllowed;
	}

	/// \brief Returns the value passed to SetMaximumIncomingConnections().
	/// \return Maximum number of incoming connections, which is always <= maxConnections
	 u32 getMaximumIncomingConnections() const {
		return maximumIncomingConnections;
	}


	/// \brief Returns how many open connections exist at this time.
	/// \return Number of open connections.
	u32 numberOfConnections() const;

	/// \brief Sets the password for the incoming connections. 
	/// \details  The password must match in the call to Connect (defaults to none).
	/// Pass 0 to passwordData to specify no password.
	/// This is a way to set a low level password for all incoming connections.  To selectively reject connections, implement your own scheme using CloseConnection() to remove unwanted connections.
	/// \param[in] passwordData A data block that incoming connections must match.  This can be just a password, or can be a stream of data. Specify 0 for no password data
	/// \param[in] passwordDataLength The length in bytes of passwordData
	void setIncomingPassword( cchar* passwordData, int passwordDataLength );

	/// \brief Gets the password passed to SetIncomingPassword
	/// \param[out] passwordData  Should point to a block large enough to hold the password data you passed to SetIncomingPassword()
	/// \param[in,out] passwordDataLength Maximum size of the passwordData array.  Modified to hold the number of bytes actually written.
	void getIncomingPassword( char* passwordData, int *passwordDataLength  );

	/// \brief Connect to the specified host (ip or domain name) and server port.
	/// \details Calling Connect and not calling SetMaximumIncomingConnections acts as a dedicated client.
	/// Calling both acts as a true peer.
	///	
	/// This is a non-blocking connection.
	///
	/// The connection is successful when GetConnectionState() returns IS_CONNECTED or Receive() gets a message with the type identifier ID_CONNECTION_REQUEST_ACCEPTED.
	/// If the connection is not successful, such as a rejected connection or no response then neither of these things will happen.
	/// \pre Requires that you first call Startup().
	/// \param[in] host Either a dotted IP address or a domain name.
	/// \param[in] remotePort Port to connect to on the remote machine.
	/// \param[in] passwordData A data block that must match the data block on the server passed to SetIncomingPassword().  This can be a string or can be a stream of data.  Use 0 for no password.
	/// \param[in] passwordDataLength The length in bytes of passwordData.
	/// \param[in] publicKey The public key the server is using. If 0, the server is not using security. If non-zero, the publicKeyMode member determines how to connect
	/// \param[in] connectionSocketIndex Index into the array of socket descriptors passed to socketDescriptors in NetPeer::Startup() to determine the one to send on.
	/// \param[in] sendConnectionAttemptCount Number of datagrams to send to the other system to try to connect.
	/// \param[in] timeBetweenSendConnectionAttemptsMS Time to elapse before a datagram is sent to the other system to try to connect. After sendConnectionAttemptCount number of attempts, ID_CONNECTION_ATTEMPT_FAILED is returned. Under low bandwidth conditions with multiple simultaneous outgoing connections, this value should be raised to 1000 or higher, or else the MTU detection can overrun the available bandwidth.
	/// \param[in] timeoutTime Time to elapse before dropping the connection if a reliable message could not be sent. 0 to use the default value from SetTimeoutTime(UNASSIGNED_SYSTEM_ADDRESS);
	/// \return CONNECTION_ATTEMPT_STARTED on successful initiation. Otherwise, an appropriate enumeration indicating failure.
	/// \note CONNECTION_ATTEMPT_STARTED does not mean you are already connected!
	/// \note It is possible to immediately get back ID_CONNECTION_ATTEMPT_FAILED if you exceed the maxConnections parameter passed to Startup(). This could happen if you call CloseConnection() with sendDisconnectionNotificaiton true, then immediately call Connect() before the connection has closed.
	ConnectionAttemptResult connect(cchar* host, u16 remotePort, cchar* passwordData, 
									u8 passwordDataLength, u32 connectionSocketIndex = 0u, 
									u32 sendConnectionAttemptCount = 6u, 
									u32 timeBetweenSendConnectionAttemptsMS = 1000u, 
									TimeMS timeoutTime = 0u);

	/// \brief Connect to the specified host (ip or domain name) and server port.
	/// \param[in] host Either a dotted IP address or a domain name.
	/// \param[in] remotePort Which port to connect to on the remote machine.
	/// \param[in] passwordData A data block that must match the data block on the server passed to SetIncomingPassword().  This can be a string or can be a stream of data.  Use 0 for no password.
	/// \param[in] passwordDataLength The length in bytes of passwordData.
	/// \param[in] socket A bound socket returned by another instance of NetPeer.
	/// \param[in] sendConnectionAttemptCount Number of datagrams to send to the other system to try to connect.
	/// \param[in] timeBetweenSendConnectionAttemptsMS Time to elapse before a datagram is sent to the other system to try to connect. After sendConnectionAttemptCount number of attempts, ID_CONNECTION_ATTEMPT_FAILED is returned.. Under low bandwidth conditions with multiple simultaneous outgoing connections, this value should be raised to 1000 or higher, or else the MTU detection can overrun the available bandwidth.
	/// \param[in] timeoutTime Time to elapse before dropping the connection if a reliable message could not be sent. 0 to use the default from SetTimeoutTime(UNASSIGNED_SYSTEM_ADDRESS);
	/// \return CONNECTION_ATTEMPT_STARTED on successful initiation. Otherwise, an appropriate enumeration indicating failure.
	/// \note CONNECTION_ATTEMPT_STARTED does not mean you are already connected!
	virtual ConnectionAttemptResult connectWithSocket(cchar* host, u16 remotePort, 
												cchar* passwordData, u8 passwordDataLength,
												NetSocket2* socket, 
												u32 sendConnectionAttemptCount = 6u, 
												u32 timeBetweenSendConnectionAttemptsMS = 1000u, 
												TimeMS timeoutTime = 0u);

	/// \brief Stops the network threads and closes all connections.
	/// \param[in] blockDuration Wait time(milli seconds) for all remaining messages to go out, including ID_DISCONNECTION_NOTIFICATION.  If 0, it doesn't wait at all.
	/// \param[in] orderingChannel Channel on which ID_DISCONNECTION_NOTIFICATION will be sent, if blockDuration > 0.
	/// \param[in] disconnectionNotificationPriority Priority of sending ID_DISCONNECTION_NOTIFICATION.
	/// If set to 0, the disconnection notification won't be sent.
	void shutdown(u32 blockDuration, u8 orderingChannel = 0, 
				  Packet::Priority disconnectionNotificationPriority = Packet::LOW_PRIORITY);

	/// \brief Returns true if the network thread is running.
	/// \return True if the network thread is running, False otherwise
	 bool isActive() const {
		return endThreads == false;
	}

	/// \brief Fills the array remoteSystems with the SystemAddress of all the systems we are connected to.
	/// \param[out] remoteSystems An array of SystemAddress structures, to be filled with the SystemAddresss of the systems we are connected to. Pass 0 to remoteSystems to get the number of systems we are connected to.
	/// \param[in, out] numberOfSystems As input, the size of remoteSystems array.  As output, the number of elements put into the array. 
	bool getConnectionList(SystemAddress* remoteSystems, u16* numberOfSystems) const;

	/// Returns the next u32 that Send() will return
	/// \note If using NetPeer from multiple threads, this may not be accurate for your thread. Use IncrementNextSendReceipt() in that case.
	/// \return The next u32 that Send() or SendList will return
	virtual u32 GetNextSendReceipt(void);

	/// Returns the next u32 that Send() will return, and increments the value by one
	/// \note If using NetPeer from multiple threads, pass this to forceReceipt in the send function
	/// \return The next u32 that Send() or SendList will return
	virtual u32 IncrementNextSendReceipt(void);

	/// \brief Sends a block of data to the specified system that you are connected to.
	/// \note This function only works while connected.
	/// \note The first byte should be a message identifier starting at ID_USER_PACKET_ENUM.
	/// \param[in] data Block of data to send.
	/// \param[in] length Size in bytes of the data to send.
	/// \param[in] priority Priority level to send on.  See Packet::Priority.h
	/// \param[in] reliability How reliably to send this data.  See Packet::Priority.h
	/// \param[in] orderingChannel When using ordered or sequenced messages, the channel to order these on. Messages are only ordered relative to other messages on the same stream.
	/// \param[in] systemIdentifier Who to send this packet to, or in the case of broadcasting who not to send it to. Pass either a SystemAddress structure or a JinraGUID structure. Use UNASSIGNED_SYSTEM_ADDRESS or to specify none
	/// \param[in] broadcast True to send this packet to all connected systems. If true, then systemAddress specifies who not to send the packet to.
	/// \param[in] forceReceipt If 0, will automatically determine the receipt number to return. If non-zero, will return what you give it.
	/// \return 0 on bad input. Otherwise a number that identifies this message. If \a reliability is a type that returns a receipt, on a later call to Receive() you will get ID_SND_RECEIPT_ACKED or ID_SND_RECEIPT_LOSS with bytes 1-4 inclusive containing this number
	u32 send(cchar *data, const u32 length, Packet::Priority priority, 
			 Packet::Reliability reliability, char orderingChannel, 
			 const AddressOrGUID systemIdentifier, bool broadcast, u32 forceReceiptNumber = 0u);

	/// \brief "Send" to yourself rather than a remote system.
	/// \details The message will be processed through the plugins and returned to the game as usual.
	/// This function works anytime
	/// \note The first byte should be a message identifier starting at ID_USER_PACKET_ENUM
	/// \param[in] data Block of data to send.
	/// \param[in] length Size in bytes of the data to send.
	void sendLoopback(cchar* data, const u32 length);

	/// \brief Sends a block of data to the specified system that you are connected to.
	/// 
	/// Same as the above version, but takes a BitStream as input.
	/// \param[in] bitStream Bitstream to send
	/// \param[in] priority Priority level to send on.  See Packet::Priority.h
	/// \param[in] reliability How reliably to send this data.  See Packet::Priority.h
	/// \param[in] orderingChannel Channel to order the messages on, when using ordered or sequenced messages. Messages are only ordered relative to other messages on the same stream.
	/// \param[in] systemIdentifier System Address or JinraGUID to send this packet to, or in the case of broadcasting, the address not to send it to.  Use UNASSIGNED_SYSTEM_ADDRESS to specify none.
	/// \param[in] broadcast True to send this packet to all connected systems. If true, then systemAddress specifies who not to send the packet to.
	/// \param[in] forceReceipt If 0, will automatically determine the receipt number to return. If non-zero, will return what you give it.
	/// \return 0 on bad input. Otherwise a number that identifies this message. If \a reliability is a type that returns a receipt, on a later call to Receive() you will get ID_SND_RECEIPT_ACKED or ID_SND_RECEIPT_LOSS with bytes 1-4 inclusive containing this number
	/// \note COMMON MISTAKE: When writing the first byte, bitStream->Write((u8) ID_MY_TYPE) be sure it is casted to a byte, and you are not writing a 4 byte enumeration.
	u32 send(const BitStream* bitStream, Packet::Priority priority, Packet::Reliability reliability,
			 char orderingChannel, const AddressOrGUID systemIdentifier, bool broadcast, 
			 u32 forceReceiptNumber = 0u);

	/// \brief Sends multiple blocks of data, concatenating them automatically.
	///
	/// This is equivalent to:
	/// Jinra::BitStream bs;
	/// bs.WriteAlignedBytes(block1, blockLength1);
	/// bs.WriteAlignedBytes(block2, blockLength2);
	/// bs.WriteAlignedBytes(block3, blockLength3);
	/// Send(&bs, ...)
	///
	/// This function only works when connected.
	/// \param[in] data An array of pointers to blocks of data
	/// \param[in] lengths An array of integers indicating the length of each block of data
	/// \param[in] numParameters Length of the arrays data and lengths
	/// \param[in] priority Priority level to send on.  See Packet::Priority.h
	/// \param[in] reliability How reliably to send this data.  See Packet::Priority.h
	/// \param[in] orderingChannel Channel to order the messages on, when using ordered or sequenced messages. Messages are only ordered relative to other messages on the same stream.
	/// \param[in] systemIdentifier System Address or JinraGUID to send this packet to, or in the case of broadcasting, the address not to send it to.  Use UNASSIGNED_SYSTEM_ADDRESS to specify none.
	/// \param[in] broadcast True to send this packet to all connected systems. If true, then systemAddress specifies who not to send the packet to.
	/// \param[in] forceReceipt If 0, will automatically determine the receipt number to return. If non-zero, will return what you give it.
	/// \return 0 on bad input. Otherwise a number that identifies this message. If \a reliability is a type that returns a receipt, on a later call to Receive() you will get ID_SND_RECEIPT_ACKED or ID_SND_RECEIPT_LOSS with bytes 1-4 inclusive containing this number
	u32 SendList(cchar** data, const s32* lengths, const s32 numParameters, 
				 Packet::Priority priority, Packet::Reliability reliability, 
				 char orderingChannel, const AddressOrGUID systemIdentifier,
				 bool broadcast, u32 forceReceiptNumber = 0u);

	/// \brief Gets a message from the incoming message queue.
	/// \details Use DeallocatePacket() to deallocate the message after you are done with it.
	/// User-thread functions, such as RPC calls and the plugin function PluginInterface::Update occur here.
	/// \return 0 if no packets are waiting to be handled, otherwise a pointer to a packet.
	/// \note COMMON MISTAKE: Be sure to call this in a loop, once per game tick, until it returns 0. If you only process one packet per game tick they will buffer up.
	/// \sa NetTypes.h contains struct Packet.
	Packet* readMessage();

	/// \brief Call this to deallocate a message returned by Receive() when you are done handling it.
	/// \param[in] packet Message to deallocate.	
	void deallocate(Packet* packet);

	/// \brief Return the total number of connections we are allowed.
	/// \return Total number of connections allowed.
	 u32 getMaximumNumberOfPeers() const {
		return maximumNumberOfPeers;
	}

	// -------------------------------------------------------------------------------------------- Connection Management Functions--------------------------------------------------------------------------------------------
	/// \brief Close the connection to another host (if we initiated the connection it will disconnect, if they did it will kick them out).
	/// \details This method closes the connection irrespective of who initiated the connection.
	/// \param[in] target Which system to close the connection to.
	/// \param[in] sendDisconnectionNotification True to send ID_DISCONNECTION_NOTIFICATION to the recipient.  False to close it silently.
	/// \param[in] channel Which ordering channel to send the disconnection notification on, if any
	/// \param[in] disconnectionNotificationPriority Priority to send ID_DISCONNECTION_NOTIFICATION on.
	void closeConnection( const AddressOrGUID target, bool sendDisconnectionNotification, u8 orderingChannel=0, Packet::Priority disconnectionNotificationPriority=Packet::LOW_PRIORITY );

	/// \brief Cancel a pending connection attempt.
	/// \details If we are already connected, the connection stays open
	/// \param[in] target Target system to cancel.
	void cancelConnectionAttempt( const SystemAddress target );
	/// Returns if a system is connected, disconnected, connecting in progress, or various other states
	/// \param[in] systemIdentifier The system we are referring to
	/// \note This locks a mutex, do not call too frequently during connection attempts or the attempt will take longer and possibly even timeout
	/// \return What state the remote system is in
	ConnectionState getConnectionState(const AddressOrGUID systemIdentifier);

	/// \brief Given \a systemAddress, returns its index into remoteSystemList.
	/// \details Values range from 0 to the maximum number of players allowed - 1.
	/// This includes systems which were formerly connected, but are now not connected.
	/// \param[in] systemAddress The SystemAddress we are referring to
	/// \return The index of this SystemAddress or -1 on system not found.
	int GetIndexFromSystemAddress( const SystemAddress systemAddress ) const;

	/// \brief Given \a index into remoteSystemList, will return a SystemAddress.
	/// This function is only useful for looping through all systems.
	/// 
	/// \param[in] index Index should range between 0 and the maximum number of players allowed - 1.
	/// \return The SystemAddress structure corresponding to \a index in remoteSystemList.
	SystemAddress GetSystemAddressFromIndex( u32 index );

	/// \brief Same as GetSystemAddressFromIndex but returns JinraGUID
	/// \param[in] index Index should range between 0 and the maximum number of players allowed - 1.
	/// \return The JinraGUID
	NetGUID GetGUIDFromIndex( u32 index );

	/// \brief Same as calling GetSystemAddressFromIndex and GetGUIDFromIndex for all systems, but more efficient
	/// Indices match each other, so \a addresses[0] and \a guids[0] refer to the same system
	/// \param[out] addresses All system addresses. Size of the list is the number of connections. Size of the \a addresses list will match the size of the \a guids list.
	/// \param[out] guids All guids. Size of the list is the number of connections. Size of the list will match the size of the \a addresses list.
	void getSystemList(std::vector<SystemAddress>& addresses, std::vector<NetGUID>& guids) const;

	/// \brief Bans an IP from connecting.
	/// \details Banned IPs persist between connections but are not saved on shutdown nor loaded on startup.
	/// \param[in] IP Dotted IP address. You can use * for a wildcard address, such as 128.0.0. * will ban all IP addresses starting with 128.0.0.
	/// \param[in] milliseconds Gives time in milli seconds for a temporary ban of the IP address.  Use 0 for a permanent ban.
	void AddToBanList(cchar* IP, TimeMS milliseconds = 0u);

	/// \brief Allows a previously banned IP to connect. 
	/// param[in] Dotted IP address. You can use * as a wildcard. An IP such as 128.0.0.* will ban all IP addresses starting with 128.0.0.
	void RemoveFromBanList(cchar* IP);

	/// \brief Allows all previously banned IPs to connect.
	void clearBanList();

	/// \brief Returns true or false indicating if a particular IP is banned.
	/// \param[in] IP Dotted IP address.
	/// \return True if IP matches any IPs in the ban list, accounting for any wildcards. False otherwise.
	bool IsBanned(cchar* IP);

	/// \brief Enable or disable allowing frequent connections from the same IP adderss
	/// \details This is a security measure which is disabled by default, but can be set to true to prevent attackers from using up all connection slots.
	/// \param[in] b True to limit connections from the same ip to at most 1 per 100 milliseconds.
	void SetLimitIPConnectionFrequency(bool b);
	
	// --------------------------------------------------------------------------------------------Pinging Functions - Functions dealing with the automatic ping mechanism--------------------------------------------------------------------------------------------
	/// Send a ping to the specified connected system.
	/// \pre The sender and recipient must already be started via a successful call to Startup()
	/// \param[in] target Which system to ping
	void ping( const SystemAddress target );

	/// \brief Send a ping to the specified unconnected system. 
	/// \details The remote system, if it is Initialized, will respond with ID_PONG followed by sizeof(Jinra::TimeMS) containing the system time the ping was sent. Default is 4 bytes - See __GET_TIME_64BIT in NetTypes.h
	/// System should reply with ID_PONG if it is active
	/// \param[in] host Either a dotted IP address or a domain name.  Can be 255.255.255.255 for LAN broadcast.
	/// \param[in] remotePort Which port to connect to on the remote machine.
	/// \param[in] onlyReplyOnAcceptingConnections Only request a reply if the remote system is accepting connections
	/// \param[in] connectionSocketIndex Index into the array of socket descriptors passed to socketDescriptors in NetPeer::Startup() to send on.
	/// \return true on success, false on failure (unknown hostname)
	bool ping( cchar* host, u16 remotePort, bool onlyReplyOnAcceptingConnections, unsigned connectionSocketIndex=0 );

	/// \brief Returns the average of all ping times read for the specific system or -1 if none read yet
	/// \param[in] systemAddress Which system we are referring to
	/// \return The ping time for this system, or -1
	int GetAveragePing( const AddressOrGUID systemIdentifier );

	/// \brief Returns the last ping time read for the specific system or -1 if none read yet.
	/// \param[in] systemAddress Which system we are referring to
	/// \return The last ping time for this system, or -1.
	int GetLastPing( const AddressOrGUID systemIdentifier ) const;

	/// \brief Returns the lowest ping time read or -1 if none read yet.
	/// \param[in] systemIdentifier Which system we are referring to
	/// \return The lowest ping time for this system, or -1.
	int GetLowestPing( const AddressOrGUID systemIdentifier ) const;

	/// Ping the remote systems every so often, or not. Can be called anytime.
	/// By default this is true. Recommended to leave on, because congestion control uses it to determine how often to resend lost packets.
	/// It would be true by default to prevent timestamp drift, since in the event of a clock spike, the timestamp deltas would no longer be accurate
	/// \param[in] doPing True to start occasional pings.  False to stop them.
	void SetOccasionalPing( bool doPing );

	/// Return the clock difference between your system and the specified system
	/// Subtract GetClockDifferential() from a time returned by the remote system to get that time relative to your own system
	/// Returns 0 if the system is unknown
	/// \param[in] systemIdentifier Which system we are referring to
	Time GetClockDifferential( const AddressOrGUID systemIdentifier );
	
	// --------------------------------------------------------------------------------------------Static Data Functions - Functions dealing with API defined synchronized memory--------------------------------------------------------------------------------------------
	/// \brief Sets the data to send along with a LAN server discovery or offline ping reply.
	/// \param[in] data Block of data to send, or 0 for none
	/// \param[in] length Length of the data in bytes, or 0 for none
	/// \note \a length should be under 400 bytes, as a security measure against flood attacks
	/// \sa Ping.cpp
	void SetOfflinePingResponse( cchar *data, const u32 length );

	/// \brief Returns pointers to a copy of the \a data passed to SetOfflinePingResponse.
	/// \param[out] data A pointer to a copy of the data passed to SetOfflinePingResponse()
	/// \param[out] length A pointer filled in with the length parameter passed to SetOfflinePingResponse()
	/// \sa SetOfflinePingResponse
	void GetOfflinePingResponse( char **data, u32 *length );
	
	//--------------------------------------------------------------------------------------------Network Functions - Functions dealing with the network in general--------------------------------------------------------------------------------------------
	/// \brief Returns the unique address identifier that represents you or another system on the the network
	/// \param[in] systemAddress Use UNASSIGNED_SYSTEM_ADDRESS to get your behind-LAN address. Use a connected system to get their behind-LAN address. This does not return the port.
	/// \param[in] index When you have multiple internal IDs, which index to return? Currently limited to MAXIMUM_NUMBER_OF_INTERNAL_IDS (so the maximum value of this variable is MAXIMUM_NUMBER_OF_INTERNAL_IDS-1)
	/// \return Identifier of your system internally, which may not be how other systems see if you if you are behind a NAT or proxy.
	SystemAddress GetInternalID( const SystemAddress systemAddress=UNASSIGNED_SYSTEM_ADDRESS, const int index=0 ) const;

	/// \brief Sets your internal IP address, for platforms that do not support reading it, or to override a value
	/// \param[in] systemAddress. The address to set. Use SystemAddress::FromString() if you want to use a dotted string
	/// \param[in] index When you have multiple internal IDs, which index to set?
	void SetInternalID(SystemAddress systemAddress, int index=0);

	/// \brief Returns the unique address identifier that represents the target on the the network and is based on the target's external IP / port.
	/// \param[in] target The SystemAddress of the remote system. Usually the same for all systems, unless you have two or more network cards.
	SystemAddress GetExternalID( const SystemAddress target ) const;

	/// Return my own GUID
	 const NetGUID getMyGUID() const {
		return myGuid;
	}

	/// Return the address bound to a socket at the specified index
	SystemAddress getMyBoundAddress(const int socketIndex=0);

	/// \brief  Given a connected system address, this method gives the unique GUID representing that instance of NetPeer.
	/// This will be the same on all systems connected to that instance of NetPeer, even if the external system addresses are different.
	/// Complexity is O(log2(n)).
	/// If \a input is UNASSIGNED_SYSTEM_ADDRESS, will return your own GUID
	/// \pre Call Startup() first, or the function will return UNASSIGNED_JINRA_GUID
	/// \param[in] input The system address of the target system we are connected to.
	const NetGUID& GetGuidFromSystemAddress( const SystemAddress input ) const;

	/// \brief Gives the system address of a connected system, given its GUID.
	/// The GUID will be the same on all systems connected to that instance of NetPeer, even if the external system addresses are different.
	/// Currently O(log(n)), but this may be improved in the future
	/// If \a input is UNASSIGNED_JINRA_GUID, UNASSIGNED_SYSTEM_ADDRESS is returned.
	/// \param[in] input The JinraGUID of the target system.
	SystemAddress GetSystemAddressFromGuid( const NetGUID input ) const;

	/// \brief Set the time, in MS, to use before considering ourselves disconnected after not being able to deliver a reliable message.

	/// Set the time, in MS, to use before considering ourselves disconnected after not being able to deliver a reliable message.
	/// Default time is 10,000 or 10 seconds in release and 30,000 or 30 seconds in debug.
	/// Do not set different values for different computers that are connected to each other, or you won't be able to reconnect after ID_CONNECTION_LOST
    /// \param[in] timeMS Time, in MS
	/// \param[in] target SystemAddress structure of the target system. Pass UNASSIGNED_SYSTEM_ADDRESS for all systems.
	void setTimeoutTime(TimeMS timeMS, const SystemAddress target);
	
	/// \brief Returns the Timeout time for the given system.
	/// \param[in] target Target system to get the TimeoutTime for. Pass UNASSIGNED_SYSTEM_ADDRESS to get the default value.
	/// \return Timeout time for a given system.
	TimeMS GetTimeoutTime(const SystemAddress target);

	/// \brief Returns the current MTU size
	/// \param[in] target Which system to get MTU for.  UNASSIGNED_SYSTEM_ADDRESS to get the default
	/// \return The current MTU size of the target system.
	s32 GetMTUSize(const SystemAddress target) const;

	/// \brief Returns the number of IP addresses this system has internally.
	/// \details Get the actual addresses from GetLocalIP()
	u32 GetNumberOfAddresses();

	/// Returns an IP address at index 0 to GetNumberOfAddresses-1 in ipList array.
	/// \param[in] index index into the list of IP addresses
	/// \return The local IP address at this index
	cchar* GetLocalIP( u32 index );

	/// Is this a local IP?
	/// Checks if this ip is in the ipList array.
	/// \param[in] An IP address to check, excluding the port.
	/// \return True if this is one of the IP addresses returned by GetLocalIP
	bool isLocalIP(cchar* ip);

	/// \brief Allow or disallow connection responses from any IP. 
	/// \details Normally this should be false, but may be necessary when connecting to servers with multiple IP addresses.
	/// \param[in] allow - True to allow this behavior, false to not allow. Defaults to false. Value persists between connections.
	void AllowConnectionResponseIPMigration( bool allow );

	/// \brief Sends a one byte message ID_ADVERTISE_SYSTEM to the remote unconnected system.
	/// This will send our external IP outside the LAN along with some user data to the remote system.
	/// \pre The sender and recipient must already be started via a successful call to Initialize
	/// \param[in] host Either a dotted IP address or a domain name
	/// \param[in] remotePort Which port to connect to on the remote machine.
	/// \param[in] data Optional data to append to the packet.
	/// \param[in] dataLength Length of data in bytes.  Use 0 if no data.
	/// \param[in] connectionSocketIndex Index into the array of socket descriptors passed to socketDescriptors in NetPeer::Startup() to send on.
	/// \return False if IsActive()==false or the host is unresolvable. True otherwise.
	bool AdvertiseSystem( cchar *host, u16 remotePort, cchar *data, int dataLength, unsigned connectionSocketIndex=0 );

	/// \brief Controls how often to return ID_DOWNLOAD_PROGRESS for large message downloads.
	/// \details ID_DOWNLOAD_PROGRESS is returned to indicate a new partial message chunk, roughly the MTU size, has arrived.
	/// As it can be slow or cumbersome to get this notification for every chunk, you can set the interval at which it is returned.
	/// Defaults to 0 (never return this notification).
	/// \param[in] interval How many messages to use as an interval before a download progress notification is returned.
	void SetSplitMessageProgressInterval(int interval);

	/// \brief Returns what was passed to SetSplitMessageProgressInterval().
	/// \return Number of messages to be recieved before a download progress notification is returned. Default to 0.
	s32 GetSplitMessageProgressInterval() const {
		return splitMessageProgressInterval;
	}

	/// \brief Set how long to wait before giving up on sending an unreliable message.
	/// Useful if the network is clogged up.
	/// Set to 0 or less to never timeout.  Defaults to 0.
	/// \param[in] timeoutMS How many ms to wait before simply not sending an unreliable message.
	void SetUnreliableTimeout(Jinra::TimeMS timeoutMS);

	// --------------------------------------------------------------------------------------------Miscellaneous Functions--------------------------------------------------------------------------------------------
	/// \brief Puts a message back in the receive queue in case you don't want to deal with it immediately.
	/// \param[in] packet The pointer to the packet you want to push back.
	/// \param[in] pushAtHead True to push the packet at the start of the queue so that the next receive call returns it.  False to push it at the end of the queue.
	/// \note Setting pushAtHead to false end makes the packets out of order.
	void PushBackPacket( Packet *packet, bool pushAtHead );

	/// \internal
	/// \brief For a given system identified by \a guid, change the SystemAddress to send to.
	/// \param[in] guid The connection we are referring to
	/// \param[in] systemAddress The new address to send to
	void ChangeSystemAddress(NetGUID guid, const SystemAddress &systemAddress);

	/// \brief Returns a packet for you to write to if you want to create a Packet for some reason.
	/// You can add it to the receive buffer with PushBackPacket
	/// \param[in] dataSize How many bytes to allocate for the buffer
	/// \return A packet.
	Packet* allocatePacket(u32 dataSize);

	Packet* allocatePacket(u32 dataSize, u8* packetData);

	/// \brief Get the socket used with a particular active connection.
	/// The smart pointer reference counts the JinraSocket object, so the socket will remain active as long as the smart pointer does, even if Jinra were to shutdown or close the connection.
	/// \note This sends a query to the thread and blocks on the return value for up to one second. In practice it should only take a millisecond or so.
	/// \param[in] target Which system.
	/// \return A smart pointer object containing the socket information about the target. Be sure to check IsNull() which is returned if the update thread is unresponsive, shutting down, or if this system is not connected.
	virtual NetSocket2* getSocket( const SystemAddress target );

	/// \brief Gets all sockets in use.
	/// \note This sends a query to the thread and blocks on the return value for up to one second. In practice it should only take a millisecond or so.
	/// \param[out] sockets List of JinraSocket structures in use.
	virtual void getSockets(std::vector<NetSocket2*>& sockets );
	virtual void ReleaseSockets(std::vector<NetSocket2*>& sockets );

	/// \internal
	virtual void WriteOutOfBandHeader(Jinra::BitStream *bitStream);

	/// If you need code to run in the same thread as Jinra's update thread, this function can be used for that
	/// \param[in] _userUpdateThreadPtr C callback function
	/// \param[in] _userUpdateThreadData Passed to C callback function
	virtual void SetUserUpdateThread(void (*_userUpdateThreadPtr)(NetPeer *, void *), void *_userUpdateThreadData);

	/// Set a C callback to be called whenever a datagram arrives
	/// Return true from the callback to have NetPeer handle the datagram. Return false and NetPeer will ignore the datagram.
	/// This can be used to filter incoming datagrams by system, or to share a recvfrom socket with NetPeer
	/// NetSocket2::RecvStruct will only remain valid for the duration of the call
	virtual  void setIncomingDatagramEventHandler(
		bool (*_incomingDatagramEventHandler)(NetSocket2::RecvStruct*)) { 
		incomingDatagramEventHandler = _incomingDatagramEventHandler;
	}

	// --------------------------------------------------------------------------------------------Network Simulator Functions--------------------------------------------------------------------------------------------
	/// Adds simulated ping and packet loss to the outgoing data flow.
	/// To simulate bi-directional ping and packet loss, you should call this on both the sender and the recipient, with half the total ping and packetloss value on each.
	/// You can exclude network simulator code with the _RELEASE #define to decrease code size
	/// \deprecated Use http://www.jenkinssoftware.com/forum/index.php?topic=1671.0 instead.
	/// \note Doesn't work past version 3.6201
	/// \param[in] packetloss Chance to lose a packet. Ranges from 0 to 1.
	/// \param[in] minExtraPing The minimum time to delay sends.
	/// \param[in] extraPingVariance The additional random time to delay sends.
	virtual void ApplyNetworkSimulator( float packetloss, u16 minExtraPing, u16 extraPingVariance);

	/// Limits how much outgoing bandwidth can be sent per-connection.
	/// This limit does not apply to the sum of all connections!
	/// Exceeding the limit queues up outgoing traffic
	/// \param[in] maxBitsPerSecond Maximum bits per second to send.  Use 0 for unlimited (default). Once set, it takes effect immedately and persists until called again.
	virtual void setPerConnectionOutgoingBandwidthLimit(u32 maxBitsPerSecond) {
		maxOutgoingBPS = maxBitsPerSecond;
	}

	/// Returns if you previously called ApplyNetworkSimulator
	/// \return If you previously called ApplyNetworkSimulator
	virtual bool isNetworkSimulatorActive();

	// --------------------------------------------------------------------------------------------Statistical Functions - Functions dealing with API performance--------------------------------------------------------------------------------------------

	/// \brief Returns a structure containing a large set of network statistics for the specified system.
	/// You can map this data to a string using the C style StatisticsToString() function
	/// \param[in] systemAddress Which connected system to get statistics for.
	/// \param[in] rns If you supply this structure,the network statistics will be written to it. Otherwise the method uses a static struct to write the data, which is not threadsafe.
	/// \return 0 if the specified system can't be found. Otherwise a pointer to the struct containing the specified system's network statistics.
	/// \sa JinraStatistics.h
	NetStatistics * GetStatistics( const SystemAddress systemAddress, NetStatistics *rns=0 );
	
	/// \brief Returns the network statistics of the system at the given index in the remoteSystemList.
	///	\return True if the index is less than the maximum number of peers allowed and the system is active. False otherwise.
	bool GetStatistics(const u32 index, NetStatistics* rns);
	
	/// \brief Returns the list of systems, and statistics for each of those systems
	/// Each system has one entry in each of the lists, in the same order
	/// \param[out] addresses SystemAddress for each connected system
	/// \param[out] guids JinraGUID for each connected system
	/// \param[out] statistics Calculated JinraStatistics for each connected system
	virtual void getStatisticsList(std::vector<SystemAddress>& addresses, 
								   std::vector<NetGUID>& guids, 
								   std::vector<NetStatistics>& statistics);

	/// \Returns how many messages are waiting when you call Receive()
	virtual u32 GetReceiveBufferSize();

	/// \internal
	bool RunUpdateCycle(BitStream &updateBitStream);

	/// \internal
	bool SendOutOfBand(cchar *host, u16 remotePort, cchar *data, BitSize_t dataLength, unsigned connectionSocketIndex=0 );

	/// \internal
	/// \brief Holds the clock differences between systems, along with the ping
	struct PingAndClockDifferential {
		u16 pingTime;
		Time clockDifferential;
	};

	/// \internal
	/// \brief All the information representing a connected system
	struct RemoteSystemStruct
	{
		bool isActive; // Is this structure in use?
		SystemAddress systemAddress;  /// Their external IP on the internet
		SystemAddress myExternalSystemAddress;  /// Your external IP on the internet, from their perspective
		SystemAddress theirInternalSystemAddress[MAXIMUM_NUMBER_OF_INTERNAL_IDS];  /// Their internal IP, behind the LAN
		ReliabilityLayer reliabilityLayer;  /// The reliability layer associated with this player
		bool weInitiatedTheConnection; /// True if we started this connection via Connect.  False if someone else connected to us.
		PingAndClockDifferential pingAndClockDifferential[ PING_TIMES_ARRAY_SIZE ];  /// last x ping times and calculated clock differentials with it
		Time pingAndClockDifferentialWriteIndex;  /// The index we are writing into the pingAndClockDifferential circular buffer
		u16 lowestPing; ///The lowest ping value encountered
		Time nextPingTime;  /// When to next ping this player
		Time lastReliableSend; /// When did the last reliable send occur.  Reliable sends must occur at least once every timeoutTime/2 units to notice disconnects
		Time connectionTime; /// connection time, if active.
		NetGUID guid;
		int MTUSize;
		// Reference counted socket to send back on
		NetSocket2* netSocket;
		SystemIndex remoteSystemIndex;

		enum ConnectMode {NO_ACTION, DISCONNECT_ASAP, DISCONNECT_ASAP_SILENTLY, DISCONNECT_ON_NO_ACK, REQUESTED_CONNECTION, HANDLING_CONNECTION_REQUEST, UNVERIFIED_SENDER, CONNECTED} connectMode;
	};

	// DS_APR
	//void ProcessChromePacket(NetSocket2 *s, cchar *buffer, int dataSize, const SystemAddress& recvFromAddress, Jinra::TimeUS timeRead);
	// /DS_APR
protected:

	friend JINRA_THREAD_DECLARATION(UpdateNetworkLoop);

	friend bool processOfflineNetworkPacket( SystemAddress systemAddress, cchar *data, const int length, NetPeer *netPeer, NetSocket2* netSocket, bool *isOfflineMessage, Jinra::TimeUS timeRead );
	friend void processNetworkPacket( const SystemAddress systemAddress, cchar *data, const int length, NetPeer *netPeer, Jinra::TimeUS timeRead, BitStream &updateBitStream );
	friend void processNetworkPacket( const SystemAddress systemAddress, cchar *data, const int length, NetPeer *netPeer, NetSocket2* netSocket, Jinra::TimeUS timeRead, BitStream &updateBitStream );

	s32 GetIndexFromSystemAddress(const SystemAddress systemAddress, bool calledFromNetworkThread ) const;
	s32 GetIndexFromGuid(const NetGUID guid);

	//void RemoveFromRequestedConnectionsList( const SystemAddress systemAddress );
	// Two versions needed because some buggy compilers strip the last parameter if unused, and crashes
	ConnectionAttemptResult sendConnectionRequest(cchar* host, u16 remotePort, 
												  cchar* passwordData, u8 passwordDataLength, 
												  u32 connectionSocketIndex, u32 extraData, 
												  u32 sendConnectionAttemptCount, 
												  u32 timeBetweenSendConnectionAttemptsMS, 
												  TimeMS timeoutTime, NetSocket2* socket);
	ConnectionAttemptResult sendConnectionRequest(cchar* host, u16 remotePort, 
												  cchar* passwordData, u8 passwordDataLength, 
												  u32 connectionSocketIndex, u32 extraData, 
												  u32 sendConnectionAttemptCount, 
												  u32 timeBetweenSendConnectionAttemptsMS, 
												  TimeMS timeoutTime);
	
	///Get the reliability layer associated with a systemAddress.  
	/// \param[in] systemAddress The player identifier 
	/// \return 0 if none
	RemoteSystemStruct *getRemoteSystemFromSystemAddress( const SystemAddress systemAddress, bool calledFromNetworkThread, bool onlyActive ) const;
	NetPeer::RemoteSystemStruct *getRemoteSystem( const AddressOrGUID systemIdentifier, bool calledFromNetworkThread, bool onlyActive ) const;
	RemoteSystemStruct *GetRemoteSystemFromGUID( const NetGUID guid, bool onlyActive ) const;
	///Parse out a connection request packet
	void ParseConnectionRequestPacket( NetPeer::RemoteSystemStruct *remoteSystem, const SystemAddress &systemAddress, cchar *data, int byteSize);
	void OnConnectionRequest( NetPeer::RemoteSystemStruct *remoteSystem, Jinra::Time incomingTimestamp );
	///Send a reliable disconnect packet to this player and disconnect them when it is delivered
	void notifyAndFlagForShutdown( const SystemAddress systemAddress, bool performImmediate, u8 orderingChannel, Packet::Priority disconnectionNotificationPriority );
	
	///Returns how many remote systems initiated a connection to us
	u32 getNumberOfRemoteInitiatedConnections() const;

	///	\brief Get a free remote system from the list and assign our systemAddress to it.
	/// \note Should only be called from the update thread - not the user thread.
	/// \param[in] systemAddress	systemAddress to be assigned
	/// \param[in] connectionMode	connection mode of the RemoteSystem.
	/// \param[in] netSocket 
	/// \param[in] thisIPConnectedRecently	Is this IP connected recently? set to False;
	/// \param[in] bindingAddress	Address to be binded with the remote system
	/// \param[in] incomingMTU	MTU for the remote system
	RemoteSystemStruct * AssignSystemAddressToRemoteSystemList( const SystemAddress systemAddress, RemoteSystemStruct::ConnectMode connectionMode, NetSocket2* incomingJinraSocket, bool *thisIPConnectedRecently, SystemAddress bindingAddress, int incomingMTU, NetGUID guid);
	///	\brief Adjust the timestamp of the incoming packet to be relative to this system.
	/// \param[in] data	Data in the incoming packet.
	/// \param[in] systemAddress Sender of the incoming packet.
	void shiftIncomingTimestamp( u8 *data, const SystemAddress &systemAddress ) const;
	/// Get the most accurate clock differential for a certain player.
	/// \param[in] systemAddress The player with whose clock the time difference is calculated.
	/// \returns The clock differential for a certain player.
	Time getBestClockDifferential( const SystemAddress systemAddress ) const;

	bool IsLoopbackAddress(const AddressOrGUID &systemIdentifier, bool matchPort) const;

	SystemAddress getLoopbackAddress() const {
		return ipList[0];
	}

	///Set this to true to terminate the Peer thread execution 
	volatile bool endThreads;
	///true if the peer thread is active. 
	volatile bool isMainLoopThreadActive;
	
	bool occasionalPing;  /// Do we occasionally ping the other systems?*/
	
	///Store the maximum number of peers allowed to connect
	u32 maximumNumberOfPeers;

	//05/02/06 Just using maximumNumberOfPeers instead
	///Store the maximum number of peers able to connect, including reserved connection slots for pings, etc.
	//u16 remoteSystemListSize;
	///Store the maximum incoming connection allowed 
	u32 maximumIncomingConnections;

	BitStream offlinePingResponse;
	///Local Player ID
	// SystemAddress mySystemAddress[MAXIMUM_NUMBER_OF_INTERNAL_IDS];
	char incomingPassword[256];
	u8 incomingPasswordLength;

	/// This is an array of pointers to RemoteSystemStruct
	/// This allows us to preallocate the list when starting, so we don't have to allocate or delete at runtime.
	/// Another benefit is that is lets us add and remove active players simply by setting systemAddress
	/// and moving elements in the list by copying pointers variables without affecting running threads, even if they are in the reliability layer
	RemoteSystemStruct* remoteSystemList;
	/// activeSystemList holds a list of pointers and is preallocated to be the same size as remoteSystemList. It is updated only by the network thread, but read by both threads
	/// When the isActive member of RemoteSystemStruct is set to true or false, that system is added to this list of pointers
	/// Threadsafe because RemoteSystemStruct is preallocated, and the list is only added to, not removed from
	RemoteSystemStruct** activeSystemList;
	u32 activeSystemListSize;

	// Use a hash, with binaryAddress plus port mod length as the index
	RemoteSystemIndex **remoteSystemLookup;
	u32 remoteSystemLookupHashIndex(const SystemAddress &sa) const;
	void ReferenceRemoteSystem(const SystemAddress &sa, u32 remoteSystemListIndex);
	void DereferenceRemoteSystem(const SystemAddress &sa);
	RemoteSystemStruct* getRemoteSystem(const SystemAddress &sa) const;
	u32 getRemoteSystemIndex(const SystemAddress &sa) const;
	void clearRemoteSystemLookup(void);
	DataStructures::MemoryPool<RemoteSystemIndex> remoteSystemIndexPool;

	void AddToActiveSystemList(u32 remoteSystemListIndex);
	void RemoveFromActiveSystemList(const SystemAddress &sa);

	enum
	{
		// Only put these mutexes in user thread functions!
		requestedConnectionList_Mutex,
		offlinePingResponse_Mutex,
		NUMBER_OF_NETPEER_MUTEXES
	};
	std::mutex netPeerMutexes[NUMBER_OF_NETPEER_MUTEXES];
	///RunUpdateCycle is not thread safe but we don't need to mutex calls. Just skip calls if it is running already

	bool updateCycleIsRunning;
	///The list of people we have tried to connect to recently

	///Data that both the client and the server needs

	u32 bytesSentPerSecond, bytesReceivedPerSecond;
	u32 validationInteger;
	std::mutex incomingQueueMutex, banListMutex; 

	struct BanStruct {
		char* IP;
		TimeMS timeout; // 0 for none
	};

	struct RequestedConnectionStruct
	{
		SystemAddress systemAddress;
		Time nextRequestTime;
		u8 requestsMade;
		char* data;
		u16 dataLength;
		char outgoingPassword[256];
		u8 outgoingPasswordLength;
		u32 socketIndex;
		u32 extraData;
		u32 sendConnectionAttemptCount;
		u32 timeBetweenSendConnectionAttemptsMS;
		TimeMS timeoutTime;
		NetSocket2* socket;
		enum { CONNECT = 1 } actionToTake;
	};

	std::vector<BanStruct*> banList;

	std::deque<RequestedConnectionStruct*> requestedConnectionQueue;
	std::mutex requestedConnectionQueueMutex;

	struct BufferedCommandStruct
	{
		BitSize_t numberOfBitsToSend;
		Packet::Priority priority;
		Packet::Reliability reliability;
		char orderingChannel;
		AddressOrGUID systemIdentifier;
		bool broadcast;
		RemoteSystemStruct::ConnectMode connectionMode;
		u64 networkID;
		bool blockingCommand; // Only used for RPC
		char* data;
		bool haveJinraCloseSocket;
		u32 connectionSocketIndex;
		u32 extraSocketOptions;
		NetSocket2* socket;
		u16 port;
		u32 receipt;
		enum {BCS_SEND, BCS_CLOSE_CONNECTION, BCS_GET_SOCKET, BCS_CHANGE_SYSTEM_ADDRESS,/* BCS_USE_USER_SOCKET, BCS_REBIND_SOCKET_ADDRESS, BCS_RPC, BCS_RPC_SHIFT,*/ BCS_DO_NOTHING} command;
	};

	// Single producer single consumer queue using a linked list

	DataStructures::ThreadsafeAllocatingQueue<BufferedCommandStruct> bufferedCommands;

	std::deque<NetSocket2::RecvStruct*> bufferedPacketsFreePool;
	std::mutex bufferedPacketsFreePoolMutex;
	std::deque<NetSocket2::RecvStruct*> bufferedPacketsQueue;
	std::mutex bufferedPacketsQueueMutex;

	virtual void deallocRNS2RecvStruct(NetSocket2::RecvStruct *s);
	virtual NetSocket2::RecvStruct *allocRNS2RecvStruct();
	void PushBufferedPacket(NetSocket2::RecvStruct * p);
	NetSocket2::RecvStruct *PopBufferedPacket();

	struct SocketQueryOutput
	{
		SocketQueryOutput() {}
		~SocketQueryOutput() {}
		std::vector<NetSocket2*> sockets;
	};

	DataStructures::ThreadsafeAllocatingQueue<SocketQueryOutput> socketQueryOutput;


	bool allowIncomingConnections() const;

	void pingInternal(const SystemAddress target, bool performImmediate, Packet::Reliability reliability);
	// This stores the user send calls to be handled by the update thread.  This way we don't have thread contention over systemAddresss
	void closeConnectionInternal( const AddressOrGUID& systemIdentifier, bool sendDisconnectionNotification, bool performImmediate, u8 orderingChannel, Packet::Priority disconnectionNotificationPriority );
	void SendBuffered(cchar *data, BitSize_t numberOfBitsToSend, Packet::Priority priority, Packet::Reliability reliability, char orderingChannel, const AddressOrGUID systemIdentifier, bool broadcast, RemoteSystemStruct::ConnectMode connectionMode, u32 receipt);
	void SendBufferedList(cchar **data, const int *lengths, const int numParameters, Packet::Priority priority, Packet::Reliability reliability, char orderingChannel, const AddressOrGUID systemIdentifier, bool broadcast, RemoteSystemStruct::ConnectMode connectionMode, u32 receipt);
	bool SendImmediate(char *data, BitSize_t numberOfBitsToSend, Packet::Priority priority, Packet::Reliability reliability, char orderingChannel, const AddressOrGUID systemIdentifier, bool broadcast, bool useCallerDataAllocation, Jinra::TimeUS currentTime, u32 receipt);
	void clearBufferedCommands();
	void clearBufferedPackets();
	void clearSocketQueryOutput();
	void clearRequestedConnectionList();
	void AddPacketToProducer(Jinra::Packet *p);
	u32 generateSeedFromGuid();
	Time getClockDifferentialInt(RemoteSystemStruct* remoteSystem) const;

	int defaultMTUSize;
	bool trackFrequencyTable;

	// Smart pointer so I can return the object to the user
	std::vector<NetSocket2* > socketList;
	void derefAllSockets();
	u32 getJinraSocketFromUserConnectionSocketIndex(u32 userIndex) const;
	// Used for RPC replies
	BitStream *replyFromTargetBS;
	SystemAddress replyFromTargetPlayer;
	bool replyFromTargetBroadcast;

	TimeMS defaultTimeoutTime;

	// Generate and store a unique GUID
	void generateGUID();
	u32 GetSystemIndexFromGuid( const NetGUID input ) const;
	NetGUID myGuid;

	u32 maxOutgoingBPS;

	// Nobody would use the internet simulator in a final build.
#ifdef JINRA_DEBUG
	double _packetloss;
	u16 _minExtraPing, _extraPingVariance;
#endif
    
	/// True to allow connection accepted packets from anyone.  False to only allow these packets from servers we requested a connection to.
	bool allowConnectionResponseIPMigration;

	SystemAddress firstExternalID;
	s32 splitMessageProgressInterval;
	TimeMS unreliableTimeout;

	bool (*incomingDatagramEventHandler)(NetSocket2::RecvStruct *);

	SystemAddress ipList[ MAXIMUM_NUMBER_OF_INTERNAL_IDS ];

	bool allowInternalRouting;

	void (*userUpdateThreadPtr)(NetPeer *, void *);
	void *userUpdateThreadData;


	SignaledEvent quitAndDataEvents;
	bool limitConnectionFrequencyFromTheSameIP;

	std::mutex packetAllocationPoolMutex;
	DataStructures::MemoryPool<Packet> packetAllocationPool;

	std::mutex packetReturnMutex;
	std::deque<Packet*> packetReturnQueue;

	/// This is used to return a number to the user when they call Send identifying the message
	/// This number will be returned back with ID_SND_RECEIPT_ACKED or ID_SND_RECEIPT_LOSS and is only returned
	/// with the reliability types that contain RECEIPT in the name
	std::mutex sendReceiptSerialMutex;
	u32 sendReceiptSerial;
	void ResetSendReceipt(void);
	void OnConnectedPong(Jinra::Time sendPingTime, Jinra::Time sendPongTime, RemoteSystemStruct *remoteSystem);

	virtual void onRNS2Recv(NetSocket2::RecvStruct *recvStruct);
	void fillIPList(void);
};

} // namespace Jinra