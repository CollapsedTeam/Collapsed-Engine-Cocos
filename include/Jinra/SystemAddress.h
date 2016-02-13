#pragma once

namespace Jinra
{

struct JINRA_DLL_EXPORT SystemAddress
{
public:
	SystemAddress();
	 
	SystemAddress(cchar* str);

	SystemAddress(cchar* str, u16 port);

	/** 
	 * Copy the port from another SystemAddress structure
	 */
	void copyPort(const SystemAddress& systemAddress);

	/** 
	 * Returns if two system addresses have the same IP (port is not checked)
	 */
	bool equalsExcludingPort(const SystemAddress& systemAddress) const;

	/**
	 * sockaddr_in6 requires extra data beyond just the IP and port. 
	 * Copy that extra data from an existing SystemAddress that already has it
	 */
	void fixForIPVersion(const SystemAddress& boundAddressToSocket);
	
	/**
	 * Set the system address from a printable IP string, for example "192.0.2.1" or "2001:db8:63b3:1::3490"
	 * You can write the port as well, using the portDelineator, for example "192.0.2.1|1234"
	 * @param[in] str A printable IP string, for example "192.0.2.1" or "2001:db8:63b3:1::3490". Pass 0 for \a str to set to UNASSIGNED_SYSTEM_ADDRESS
	 * @param[in] portDelineator if \a str contains a port, delineate the port with this character. portDelineator should not be '.', ':', '%', '-', '/', a number, or a-f
	 * @param[in] ipVersion Only used if str is a pre-defined address in the wrong format, such as 127.0.0.1 but you want ip version 6, so you can pass 6 here to do the conversion
	 * \note The current port is unchanged if a port is not specified in \a str
	 * @return True on success, false on ipVersion does not match type of passed string
	 */
	bool fromString(cchar* str, char portDelineator = '|', s32 ipVersion = 0);

	/**
	 * Same as fromString(), but you explicitly set a port at the same time
	 */
	bool fromStringExplicitPort(cchar* str, u16 port, s32 ipVersion = 0);

	/** 
	 * Hash the system address.
	 * @param[in] systemAddress
	 * @return
	 */
	static unsigned long toInteger(const SystemAddress& systemAddress);

	/**
	 * Return the systemAddress as a string in the format <IP>|<Port>.
	 * @param[in] writePort
	 * @param[in] portDelineator Port delineator should not be '.', ':', '%', '-', '/', a number, or a-f
	 * @return Returns a static string.
	 */
	SString toString(bool writePort = true, char portDelineator = '|') const;

	/**
	 * Return the systemAddress as a string in the format <IP>|<Port>.
	 * @param[in] writePort
	 * @param[out] dest Destination buffer must be large enough to hold the output.
	 * @param[in] portDelineator Port delineator should not be '.', ':', '%', '-', '/', a number, or a-f
	 */
	void toString(bool writePort, char *dest, char portDelineator = '|') const;

	 bool isLANAddress() {
#if defined(__WIN32__)
		return (address.addr4.sin_addr.S_un.S_un_b.s_b1 == 10 || 
				address.addr4.sin_addr.S_un.S_un_b.s_b1 == 192);
#else // defined(__WIN32__)
		return ((address.addr4.sin_addr.s_addr >> 24) == 10 || 
				(address.addr4.sin_addr.s_addr >> 24) == 192);
#endif // defined(__WIN32__)
	}

	/**
	 *
	 * @return Return true if was set to 127.0.0.1 or ::1. False otherwise.
	 */
	bool isLoopback() const;

	/** 
	 * Return the IP version, either IPV4 or IPV6.
	 * @return Returns either 4 or 6.
	 */
	 u8 getIPVersion() const {
		return (address.addr4.sin_family == AF_INET) ? 4 : 6;
	}

	/** 
	 * Returns either IPPROTO_IP or IPPROTO_IPV6.
	 * @return 
	 */
	 u32 getIPPROTO() const {
#if defined(JINRA_SUPPORT_IPV6)
		return (address.addr4.sin_family == AF_INET) ? IPPROTO_IP : IPPROTO_IPV6;
#else // defined(JINRA_SUPPORT_IPV6)
		return IPPROTO_IP;
#endif // defined(JINRA_SUPPORT_IPV6)
	}

	/**
	 * Returns the port in host order (this is what you normally use).
	 * @return
	 */
	 u16 getPort() const {
		return ntohs(address.addr4.sin_port);
	}

	/**
	 * Returns the port in network order
	 * @return
	 */
	 u16 getPortNetworkOrder() const {
		return address.addr4.sin_port;
	}

	/** 
	 * Return the size to write to a bitStream.
	 * @return 
	 */
	 static s32 getSize() {
#if defined(JINRA_SUPPORT_IPV6)
		return sizeof(sockaddr_in6) + sizeof(s8);
#else // defined(JINRA_SUPPORT_IPV6)
		return sizeof(u32) + sizeof(u16) + sizeof(s8);
#endif // defined(JINRA_SUPPORT_IPV6)
	}

	/** 
	 * Call SetToLoopback(), with whatever IP version is currently held. Defaults to IPV4.
	 */
	 void setToLoopback() {
		setToLoopback(getIPVersion());
	}

	/** 
	 * Call SetToLoopback() with a specific IP version.
	 * @param[in] ipVersion Either 4 for IPV4 or 6 for IPV6.
	 */
	void setToLoopback(u8 ipVersion);
	
	/** 
	 * Old version, for crap platforms that don't support newer socket functions.
	 */
	bool setBinaryAddress(cchar* str, char portDelineator = ':');

	/** 
	 * Sets the port. The port value should be in host order (this is what you normally use).
	 * @param[in] port
	 */
	void setPortHostOrder(u16 port);

	/** 
	 * Sets the port. The port value should already be in network order.
	 * @param[in] port
	 */
	void setPortNetworkOrder(u16 port);

	SystemAddress& operator=(const SystemAddress& systemAddress);

	 bool operator==(const SystemAddress& systemAddress) const {
		return (address.addr4.sin_port == systemAddress.address.addr4.sin_port && 
				equalsExcludingPort(systemAddress));
	}

	 bool operator!=(const SystemAddress& systemAddress) const {
		return (*this == systemAddress) == false;
	}

	bool operator>(const SystemAddress& systemAddress) const;

	bool operator<(const SystemAddress& systemAddress) const;

public:
	union
	{
#if defined(JINRA_SUPPORT_IPV6)
		sockaddr_in6 addr6;
#endif // defined(JINRA_SUPPORT_IPV6)
		sockaddr_in addr4;
	} address;

	// This is not used internally, but holds a copy of the port held in the address 
	// union, so for debugging it's easier to check what port is being held.
	u16 debugPort;

	// Used internally for fast lookup. Optional (use -1 to do regular lookup). 
	// Don't transmit this.
	SystemIndex systemIndex;

private:
	/** 
	 * Old version, for crap platforms that don't support newer socket functions.
	 */
	void toStringOld(bool writePort, char* dest, char portDelineator = ':') const;

#if defined(JINRA_SUPPORT_IPV6)
	void toStringNew(bool writePort, char* dest, char portDelineator) const;
#endif // defined(JINRA_SUPPORT_IPV6)
};

#ifndef SWIG
const SystemAddress UNASSIGNED_SYSTEM_ADDRESS;
#endif

} // namespace Jinra