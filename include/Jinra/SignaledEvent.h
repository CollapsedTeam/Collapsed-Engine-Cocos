#pragma once

namespace Jinra
{

class JINRA_DLL_EXPORT SignaledEvent
{
public:
	SignaledEvent();

	void initEvent();

	void closeEvent();

	void waitOnEvent(s32 timeoutMS);

	void setEvent();

protected:
#if defined(_WIN32)
	HANDLE _eventList;
#else // defined(_WIN32)
	std::mutex _isSignaledMutex;
	bool _isSignaled;
	pthread_cond_t _eventList;
	pthread_mutex_t _hMutex;
	pthread_mutexattr_t _mutexAttr;
#if !defined(ANDROID)
	pthread_condattr_t _condAttr;
#endif // !defined(ANDROID)
#endif // defined(_WIN32)
};

} // namespace Jinra

