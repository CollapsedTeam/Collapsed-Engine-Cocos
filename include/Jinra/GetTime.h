#pragma once

namespace Jinra
{

/** 
 * Return the time as 32 bit.
 * \note The maximum delta between returned calls is 1 second - however, Jinra calls this 
 * constantly anyway.
 */
TimeMS JINRA_DLL_EXPORT getTimeMS();

/** 
 * Return the time as 64 bit.
 * \note The maximum delta between returned calls is 1 second - however, Jinra calls this 
 * constantly anyway.
 */
Time JINRA_DLL_EXPORT getTime();
TimeUS JINRA_DLL_EXPORT getTimeUS();

} // namespace Jinra