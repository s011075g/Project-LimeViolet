#pragma once
#include "../SDL/sdlWindow.h"
class IRenderDevice
{
private:
	static bool isInitialized;
public:
	static bool GlobalInit();
	IRenderDevice(SDLWindow*& window);
	virtual ~IRenderDevice();
};

