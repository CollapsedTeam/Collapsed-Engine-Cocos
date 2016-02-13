#pragma once

namespace Jinra
{

struct JINRA_DLL_EXPORT uint24_t
{
public:
	uint24_t() = default;

	uint24_t(const uint24_t& a);

	uint24_t(const u32& a);

	uint24_t operator++();

	uint24_t operator--();

	uint24_t operator++(s32);

	uint24_t operator--(s32);

	uint24_t operator&(const uint24_t& a);

	uint24_t& operator=(const uint24_t& a);

	uint24_t& operator+=(const uint24_t& a);

	uint24_t& operator-=(const uint24_t& a);

	bool operator==(const uint24_t& right) const;

	bool operator!=(const uint24_t& right) const;

	bool operator>(const uint24_t& right) const;

	bool operator<(const uint24_t& right) const;

	const uint24_t operator+(const uint24_t& other) const;

	const uint24_t operator-(const uint24_t& other) const;

	const uint24_t operator/(const uint24_t& other) const;

	const uint24_t operator*(const uint24_t& other) const;

	uint24_t operator&(const u32& a);

	uint24_t& operator=(const u32& a);

	uint24_t& operator+=(const u32& a);

	uint24_t& operator-=(const u32& a);

	bool operator==(const u32& right) const;

	bool operator!=(const u32& right) const;

	bool operator>(const u32& right) const;

	bool operator<(const u32& right) const;

	const uint24_t operator+(const u32& other) const;

	const uint24_t operator-(const u32& other) const;

	const uint24_t operator/(const u32& other) const;

	const uint24_t operator*(const u32& other) const;
	
	 operator u32() {
		return val;
	}

	 operator u32() const {
		return val;
	}

public:
	u32 val;
};

} // namespace Jinra