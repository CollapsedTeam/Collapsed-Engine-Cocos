#pragma once

namespace Jinra
{

// To define a thread, use JINRA_THREAD_DECLARATION(functionName);
#if defined(_WIN32)
#define JINRA_THREAD_DECLARATION(functionName) u32 __stdcall functionName(void* arguments)
#else // defined(_WIN32)
#define JINRA_THREAD_DECLARATION(functionName) void* functionName(void* arguments)
#endif // defined(_WIN32)

class JINRA_DLL_EXPORT NetThread
{
public:
	/** 
	 * Create a thread, simplified to be cross platform without all the extra junk
	 * To then start that thread, call NetThread::Create(functionName, arguments);
	 * @param start_address Function you want to call
	 * @param arglist Arguments to pass to the function
	 * @return 0 = success. > 0 = error code
	 *
	 * nice   value 	Win32 Priority
	 * -20 to -16 		THREAD_PRIORITY_HIGHEST
	 * -15 to -6 		THREAD_PRIORITY_ABOVE_NORMAL
	 * -5  to +4 		THREAD_PRIORITY_NORMAL
	 * +5  to +14 		THREAD_PRIORITY_BELOW_NORMAL
	 * +15 to +19 		THREAD_PRIORITY_LOWEST
	 */
#if defined(_WIN32)
	static s32 create(u32 __stdcall startAddress(void*), void* argList, s32 priority = 0);
#else // defined(_WIN32)
	static s32 create(void* startAddress(void*), void* argList, s32 priority = 0);
#endif // defined(_WIN32)
};

} // namespace Jinra
