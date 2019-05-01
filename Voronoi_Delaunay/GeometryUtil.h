#pragma once

#include "Vec3.h"

namespace artstealer
{
	class GeometryUtil
	{
	public:
		GeometryUtil();
		~GeometryUtil();

		static bool IsPointInTriangle(const Vec3& p, const Vec3& a, const Vec3& b, const Vec3& c);
	};
}


