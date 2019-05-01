#pragma once

//#include <list>
#include <vector>
#include <math.h>

namespace artstealer
{
	//typedef float REAL;
#ifdef SINGLE
	typedef float REAL;
#else
	typedef double REAL;
#endif

	class Vec3
	{
	public:
		Vec3() :x(0), y(0), z(0) {};
		Vec3(REAL vx, REAL vy, REAL vz) :x(vx), y(vy), z(vz) {};
		Vec3(const Vec3 &v) :x(v.x), y(v.y), z(v.z) {};

		virtual ~Vec3() {};
		REAL x;
		REAL y;
		REAL z;

		Vec3 &operator =(const Vec3 & a) {
			x = a.x; y = a.y; z = a.z;
			return *this;
		}

		bool operator ==(const Vec3 & a) const {
			return x == a.x && y == a.y && z == a.z;
		}

		bool operator !=(const Vec3 & a) const {
			return x != a.x || y != a.y || z != a.z;
		}


		// Vector operations
		void zero() { x = y = z = 0.0f; }

		Vec3 operator -() const { return Vec3(-x, -y, -z); }

		Vec3 operator +(const Vec3 & a) const {
			return Vec3(x + a.x, y + a.y, z + a.z);
		}

		Vec3 operator -(const Vec3 & a) const {
			return Vec3(x - a.x, y - a.y, z - a.z);
		}

		Vec3 operator *(float a) const {
			return Vec3(x*a, y*a, z*a);
		}

		Vec3 operator /(float a) const {
			float	oneOverA = 1.0f / a; // NOTE: no check for divide by zero here
			return Vec3(x*oneOverA, y*oneOverA, z*oneOverA);
		}

		Vec3 &operator +=(const Vec3 & a) {
			x += a.x; y += a.y; z += a.z;
			return *this;
		}

		Vec3 &operator -=(const Vec3 & a) {
			x -= a.x; y -= a.y; z -= a.z;
			return *this;
		}

		Vec3 &operator *=(float a) {
			x *= a; y *= a; z *= a;
			return *this;
		}

		Vec3 &operator /=(float a) {
			float	oneOverA = 1.0f / a;
			x *= oneOverA; y *= oneOverA; z *= oneOverA;
			return *this;
		}

		// Normalize the vector
		void normalize() {
			float magSq = x * x + y * y + z * z;
			if (magSq > 0.0f) { // check for divide-by-zero
				float oneOverMag = 1.0f / sqrt(magSq);
				x *= oneOverMag;
				y *= oneOverMag;
				z *= oneOverMag;
			}
		}

		// dot product
		float operator *(const Vec3 & a) const {
			return x * a.x + y * a.y + z * a.z;
		}

		REAL length() {
			return sqrt(x * x + y * y + z * z);
		}

		REAL lengthSQ() {
			return x * x + y * y + z * z;
		}
	};

	inline Vec3 operator *(REAL k, const Vec3 & v) {
		return Vec3(k*v.x, k*v.y, k*v.z);
	}

	inline Vec3 operator ^(const Vec3 &a, const Vec3 & b) {
		return Vec3(
			a.y*b.z - a.z*b.y,
			a.z*b.x - a.x*b.z,
			a.x*b.y - a.y*b.x
		);
	}

	inline float distance(const Vec3 & a, const Vec3 & b) {
		float dx = a.x - b.x;
		float dy = a.y - b.y;
		float dz = a.z - b.z;
		return sqrt(dx*dx + dy * dy + dz * dz);
	}

	inline float distanceSQ(const Vec3 & a, const Vec3 & b) {
		float dx = a.x - b.x;
		float dy = a.y - b.y;
		float dz = a.z - b.z;
		return dx * dx + dy * dy + dz * dz;
	}

	typedef std::vector<Vec3> PointArray;//vec3 container
}

