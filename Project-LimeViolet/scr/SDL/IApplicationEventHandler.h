#pragma once
#include <cstdint>

class IApplicationEventHandler
{
public:
	IApplicationEventHandler() = default;
	virtual ~IApplicationEventHandler() = default;
	virtual void OnKeyDown(uint32_t keyCode, bool isRepeat) {};
	virtual void OnKeyUp(uint32_t keyCode, bool isRepeat) {};
	virtual void OnMouseDown(uint32_t mouseButton, uint8_t numClicks) {};
	virtual void OnMouseUp(uint32_t mouseButton, uint8_t numClicks) {};
	virtual void OnMouseMove(int32_t mousePosX, int32_t mousePosY, int32_t deltaX, int32_t deltaY){};
private:
	IApplicationEventHandler(const IApplicationEventHandler& other) = delete;
	void operator=(const IApplicationEventHandler& other) = delete;
};
