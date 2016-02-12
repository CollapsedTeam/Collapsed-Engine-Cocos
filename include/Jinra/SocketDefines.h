#pragma once

#if defined(_WIN32)
#define closesocket__	::closesocket
#define select__		::select
#else // defined(_WIN32)
#define closesocket__	::close
#define select__		::select
#endif // defined(_WIN32)
