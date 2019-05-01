#include "GeometryUtil.h"



namespace artstealer
{

	GeometryUtil::GeometryUtil()
	{
	}


	GeometryUtil::~GeometryUtil()
	{
	}

	bool GeometryUtil::IsPointInTriangle(const Vec3 & p, const Vec3 & a, const Vec3 & b, const Vec3 & c)
	{
		Vec3 v0 = c - a;
		Vec3 v1 = b - a;
		Vec3 v2 = p - a;

		REAL dot00 = v0 * v0;
		REAL dot01 = v0 * v1;
		REAL dot02 = v0 * v2;
		REAL dot11 = v1 * v1;
		REAL dot12 = v1 * v2;

		REAL inverDeno = 1 / (dot00 * dot11 - dot01 * dot01);

		REAL u = (dot11 * dot02 - dot01 * dot12) * inverDeno;
		if (u < 0 || u > 1) // if u out of range, return directly
		{
			return false;
		}

		REAL v = (dot00 * dot12 - dot01 * dot02) * inverDeno;
		if (v < 0 || v > 1) // if v out of range, return directly
		{
			return false;
		}

		return u + v <= 1;
	}

}


