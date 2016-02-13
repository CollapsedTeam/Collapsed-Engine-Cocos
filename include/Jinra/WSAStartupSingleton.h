#pragma once

class WSAStartupSingleton
{
public:
	/**
	 * Initializes WSA - calls WSAStartup.
	 */
	static void initialize();

	/**
	 * Releases WSA - calls WSACleanup.
	 */
	static void release();

	static  bool isInitialized() {
		return _initialized;
	}

protected:
	static bool _initialized;
};
