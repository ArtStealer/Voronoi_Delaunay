#pragma once

#include <list>
#include <vector>
#include "Vec3.h"

namespace artstealer
{
	typedef struct
	{
		int vi1, vi2; // index of vector, two ends of the edge;
		int count; // referenct count. delete,if 0;
	}DEdge;

	typedef std::vector<DEdge> DEdgeArray; // edge container

	// DelaunayTriangle
	typedef struct 
	{
		int vi[3]; // ascending order, important
		Vec3 cp; // center point, The center of a circumcircle
		REAL radius; // radius of circumcircle
	}DTri;

	typedef std::vector<DTri> DTriangleArray; // triangle container

	class Delaunay
	{
	public:
		Delaunay(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 v4);

		virtual ~Delaunay();

		void DeleteFrame();
		bool AddPoint(const Vec3 & v);
		bool CircumCenter(int vi1, int vi2, int vi3, Vec3& center, REAL & radius);
		bool AddTriangle(int vi1, int vi2, int vi3);
		void DelTriangle(int n, DEdgeArray & edges);
		bool isInCircle(const Vec3 & p, const DTri & tri);
		bool GetCommonEdge(DTri tri1, DTri tri2, DEdge & edge);
		int GetEdge(int vi1, int vi2, DEdge & edge);

		const PointArray& GetPoints() { return mPoints; }
		const DEdgeArray& GetEdges() { return mEdges; }
		const DTriangleArray& GetTrianges() { return mTrianges; }

	private:
		PointArray mPoints;
		DEdgeArray mEdges;
		DTriangleArray mTrianges;
		int nBorder;
	};
};

