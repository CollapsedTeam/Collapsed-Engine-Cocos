#pragma once

// From http://www.azillionmonkeys.com/qed/hash.html
// Author of main code is Paul Hsieh.
// I just added some convenience functions.

namespace Jinra
{

u32 superFastHash(cchar* data, s32 length);

u32 superFastHashIncremental(cchar* data, s32 len, u32 lastHash);

u32 superFastHashFile(cchar* fileName);

u32 superFastHashFilePtr(FILE* file);

} // namespace Jinra