#pragma once

namespace Jinra
{

extern "C"
{

// Fast itoa for Linux since it seems like Linux doesn't support this function.
char* itoa(s32 value, char* result, s32 base);

} // extern "C" 

} // namespace Jinra