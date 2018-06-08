#pragma once
#include "Geometry.h"
class IVBOManager
{
public:
	IVBOManager();
	virtual ~IVBOManager();

	virtual void* VBOGeometry(Geometry * geometry) = 0;
};