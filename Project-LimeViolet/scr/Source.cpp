#include "ECS/Component.h"
#include "Maths/MathStructs.h"
#include "Common/Color.h"

#ifdef far
float aa;
#endif

#ifdef near
float bb;
#endif

struct debugStruct
{
	float near;
};

int debug()
{
	float far = 1.0f;
	return far;
}