#pragma once

#include "util.h"
#include "../CG_MATH_D3D/vector3.h"
class DistantLight
{
public:
	DistantLight();
	~DistantLight();

	color3f ia, id, is;
	CG_MATH::vector3 dir;
};

