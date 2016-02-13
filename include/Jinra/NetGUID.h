#pragma once

namespace Jinra
{

struct JINRA_DLL_EXPORT NetGUID
{
public:
	NetGUID();

	explicit NetGUID(u64 id);

	bool fromString(cchar* source);

	/** 
	 * [NOT THREADSAFE] Return the GUID as a string.
	 * @return Returns a static string.
	 */
	cchar* toString() const;

	/**
	 * [THREADSAFE] Return the GUID as a string dest must be large enough to hold the output.
	 * @param[out] dest
	 */
	void toString(char* dest) const;

	static unsigned long toUint32(const NetGUID& guid);

	static s32 getSize() { 
		return static_cast<s32>(sizeof(u64)); 
	}

	NetGUID& operator=(const NetGUID& guid);

	 bool operator==(const NetGUID& guid) const {
		return (id == guid.id);
	}

	 bool operator!=(const NetGUID& guid) const {
		return (id != guid.id);
	}

	 bool operator>(const NetGUID& guid) const {
		return (id > guid.id);
	}

	 bool operator<(const NetGUID& guid) const {
		return (id < guid.id);
	}

public:
	u64 id;

	// Used internally for fast lookup. Optional (use -1 to do regular lookup). 
	// Don't transmit this.
	SystemIndex systemIndex;
};

#ifndef SWIG
const NetGUID UNASSIGNED_NET_GUID(static_cast<u64>(-1));
#endif

} // namespace Jinra