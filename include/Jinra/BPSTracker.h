#pragma once

namespace Jinra
{

struct BPSTracker
{
public:
	struct TimeAndValue
	{
		TimeAndValue() = default;

		TimeAndValue(TimeUS time, u64 value);

		u64 value;
		TimeUS time;
	};

public:
	BPSTracker();
	
	void reset();
	
	void push(TimeUS time, u64 value);

	void clearExpired(TimeUS time);

	inline u64 getBPS() const { 
		return _lastSec; 
	}
	
	inline u64 getTotal() const {
		return _total; 
	}

private:
	u64 _total;
	u64 _lastSec;
	std::deque<TimeAndValue> _dataQueue;
};

} // namespace Jinra