#include "defines.h"

class VidorIRQ {
public:
	static void onInterrupt();
	static bool getInterruptSource(bool force = false);
	static void lock();
	static void unlock();
};