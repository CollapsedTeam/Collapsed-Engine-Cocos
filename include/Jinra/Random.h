#pragma once

namespace Jinra
{

class Random
{
public:
	/**
	 * Initializes basic seed for random generation.
	 */
	static inline void initSeed() {
#if defined(_WIN32)
		srand(static_cast<u32>(time(0)));
#else
		srand(time(0));
#endif
	}

	/**
	 * Initializes specified seed for random generation.
	 * @param seed The seed for random generation.
	 */
	static inline void initSeed(u32 seed) {
		srand(seed);
	}

	/**
	 * Generates integer.
	 */
	static inline s32 generateInt() {
		return rand();
	}

	/**
	 * Generates integer from specified interval.
	 * @param min The minimal value of interval.
	 * @param max The maximal value of interval.
	 */
	static inline s32 generateInt(s32 min, s32 max) {
		return (rand() % (max - min + 1)) + min;
	}

	/**
	 * Generates float.
	 */
	static inline float generateFloat() {
		return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
	}

	/**
	 * Generates float from specified interval.
	 * @param min The minimal value of interval.
	 * @param max The maximal value of interval.
	 */
	static inline float generateFloat(float min, float max) {
		return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) / max) + min;
	}
};

} // namespace Jinra