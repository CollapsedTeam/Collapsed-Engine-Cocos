#pragma once

#define JINRA_VERSION "1.0"
#define JINRA_VERSION_NUMBER 1.0
#define JINRA_VERSION_NUMBER_INT 1

// What compatible protocol version Jinra is using. When this value changes, it indicates 
// this version of Jinra cannot connection to an older version.
// ID_INCOMPATIBLE_PROTOCOL_VERSION will be returned on connection attempt in this case
#define JINRA_PROTOCOL_VERSION 1

#if defined(_WIN32) && !(defined(__GNUC__)  || defined(__GCCXML__)) && !defined(JINRA_LIB) && defined(JINRA_DLL)
#define JINRA_DLL_EXPORT __declspec(dllexport)
#else
#define JINRA_DLL_EXPORT  
#endif

#if defined(DEBUG) || defined(_DEBUG)
#define JINRA_DEBUG
#endif

#include "NetDefines.h"

// Redefine if you want to disable or change the target for debug JINRA_DEBUG_PRINTF
#ifndef JINRA_DEBUG_PRINTF
#define JINRA_DEBUG_PRINTF printf
#endif

/// Given a number of bits, return how many bytes are needed to represent that.
#define BITS_TO_BYTES(x) (((x)+7)>>3)
#define BYTES_TO_BITS(x) ((x)<<3)

#include <map>
#include <vector>
#include <mutex>
#include <queue>
#include <atomic>
#include <string>

#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <float.h>
#include <memory.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>

#if defined(_WIN32)
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#else // defined(_WIN32)
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h> 
#include <unistd.h>
#include <pthread.h>
#if defined(__GNUC__)  || defined(__GCCXML__)
#include <netdb.h>
#endif // defined(__GNUC__)  || defined(__GCCXML__)
#if !defined(ANDROID)
#include <ifaddrs.h>
#endif // !defined(ANDROID)
#endif // defined(_WIN32)

#if defined(__FreeBSD__)
#elif defined(__APPLE__) || defined(__APPLE_CC__)
#include <malloc/malloc.h>
#include <alloca.h>
#elif defined(_WIN32)
#include <malloc.h>
#else
#include <malloc.h>
#include <alloca.h>
#endif

typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;

typedef const char cchar;
typedef std::string StdString;
typedef StdString SString;

namespace Jinra
{

#define BITSTREAM(bitStream,packetID) \
	BitStream bitStream; \
	bitStream.write(static_cast<u8>(packetID))

#define READ(bitStream,type,var) \
	type var; \
	bitStream->read(var)

#if defined(JINRA_DEBUG)
#define ASSERT(x) assert(x);
#else
#define ASSERT(x) 
#endif

#ifndef MAXIMUM_MTU_SIZE
/// \li \em 17914 16 Mbit/Sec Token Ring
/// \li \em 4464 4 Mbits/Sec Token Ring
/// \li \em 4352 FDDI
/// \li \em 1500. The largest Ethernet packet size \b recommended. This is the typical setting for non-PPPoE, non-VPN connections. The default value for NETGEAR routers, adapters and switches.
/// \li \em 1492. The size PPPoE prefers.
/// \li \em 1472. Maximum size to use for pinging. (Bigger packets are fragmented.)
/// \li \em 1468. The size DHCP prefers.
/// \li \em 1460. Usable by AOL if you don't have large email attachments, etc.
/// \li \em 1430. The size VPN and PPTP prefer.
/// \li \em 1400. Maximum size for AOL DSL.
/// \li \em 576. Typical value to connect to dial-up ISPs.
/// The largest value for an UDP datagram
#define MINIMUM_MTU_SIZE 400
#define MAXIMUM_MTU_SIZE 1492
#endif

// First byte of a network message
typedef u8 MessageID;

typedef u16 SystemIndex;
typedef u32 BitSize_t;

#if defined(_MSC_VER) && _MSC_VER > 0
#define PRINTF_64_BIT_MODIFIER "I64"
#else
#define PRINTF_64_BIT_MODIFIER "ll"
#endif

#define JINRA_TIME_FORMAT_STRING "%" PRINTF_64_BIT_MODIFIER "u"
typedef u32 TimeMS;
typedef u64 TimeUS;
typedef TimeUS Time;

extern JINRA_DLL_EXPORT void(*notifyOutOfMemory)(cchar* file, const long line);

extern void JINRA_DLL_EXPORT setNotifyOutOfMemory(void(*userFunction)(cchar* file, 
	const long line));

extern bool ipAddressMatch(const SString &string, cchar* IP);

} // namespace Jinra

