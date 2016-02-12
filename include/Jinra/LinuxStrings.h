#pragma once

#if (defined(__GNUC__) || defined(__GCCXML__) || defined(__S3E__)) && !defined(_WIN32)

#ifndef _stricmp
s32 _stricmp(cchar* s1, cchar* s2);
#endif // _stricmp

s32 _strnicmp(cchar* s1, cchar* s2, size_t n);

// http://www.jenkinssoftware.com/forum/index.php?topic=5010.msg20920#msg20920
// #ifndef _vsnprintf
#define _vsnprintf vsnprintf
// #endif

#endif // (defined(__GNUC__) || defined(__GCCXML__) || defined(__S3E__)) && !defined(_WIN32)


