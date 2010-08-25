#if !defined(MBK_PLATFORM_INCLUDED)
#define MBK_PLATFORM_INCLUDED
#include <string>

class Platform {
public:
	static void vibrate();
public:
	static float timeAsSeconds();
	static float timeElapsedInSeconds(const float& reference);
};

#endif // MBK_PLATFORM_INCLUDED
