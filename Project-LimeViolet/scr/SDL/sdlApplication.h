#pragma once
#include <cstdint>
#include "IApplicationEventHandler.h"
class SDLApplication
{
private:
	bool _isAppRunning;
	static uint32_t numInstances;
public:
	static SDLApplication* Create();
	virtual ~SDLApplication();
	virtual void ProcessMessages(double delta, IApplicationEventHandler& eventHandler);
	virtual bool IsRunning() const;
private:
	SDLApplication();
};

