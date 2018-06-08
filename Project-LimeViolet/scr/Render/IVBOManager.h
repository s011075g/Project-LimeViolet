#pragma once
#include "Geometry.h"
#include "RawGeometry.h"

class IVBOManager
{
public:
	IVBOManager();
	virtual ~IVBOManager();

	virtual Geometry * VBOGeometry(RawGeometry * geometry) = 0;
	virtual void FreeVBO(Geometry* geometry) = 0;
};
