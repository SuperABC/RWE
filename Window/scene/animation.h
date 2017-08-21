#ifndef RWE_PLUGIN_MANAGER_REGISTER
#define RWE_PLUGIN_MANAGER_REGISTER
#include "Frame/main/main.h"

class Animation {
private:
	bool finish = false;
public:
	bool start = false;
	void(*idle)();
};


#endif