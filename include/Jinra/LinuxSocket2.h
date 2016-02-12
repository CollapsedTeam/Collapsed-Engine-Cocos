#pragma once

#if !defined(_WIN32)

#include "BerkleySocket2.h"

namespace Jinra
{

class LinuxSocket2 : public BerkleySocket2
{
public:
	NetSocket2::BindResult bind(BerkleySocket2::BindParameters* bindParameters) override;

	RNS2SendResult send(NetSocket2::SendParameters* sendParameters) override;

public:
	static void getMyIP(SystemAddress addresses[MAXIMUM_NUMBER_OF_INTERNAL_IDS]);

protected:
	static void getMyIPIPV4(SystemAddress addresses[MAXIMUM_NUMBER_OF_INTERNAL_IDS]);

#if defined(JINRA_SUPPORT_IPV6)
	static void getMyIPIPV4And6(SystemAddress addresses[MAXIMUM_NUMBER_OF_INTERNAL_IDS]);
#endif
};

} // namespace Jinra

#endif // !defined(_WIN32)