#pragma once

#include <vector>
#include "Delaunay.h"

namespace artstealer
{
	class VTriangle;

	class VEdge
	{
	public:
		VEdge(int v1,int v2):vi1(v1),vi2(v2) {}
		int vi1, vi2; // 对应mCCPoints数组
	};


	//circumcenter point
	class VCCPoint
	{
	public:
		Vec3 p;
		int itri; //所在三角形的索引 对应mTriangles

	};

	class VTriPoint
	{
	public:
		Vec3 v;
		std::vector<int> edges; // 对应 mTriEdges
	};

	class VTriEdge
	{
	public:
		VTriEdge() {};
		VTriEdge(const VTriEdge& rhs) 
		{
			vi1 = rhs.vi1;
			vi2 = rhs.vi2;
			adjTris = rhs.adjTris;
		};

		int vi1, vi2; // 对应mTriPoints数组
		std::vector<int> adjTris;
	};

	class VTriangle
	{
	public:
		VTriangle():iccp(0){
			memset(dedges, 0, sizeof(dedges));
			memset(vi, 0, sizeof(vi));
		}
		int iccp; // index of CircumCenter Point
		int vi[3]; // 对应 mTriPoints
		int dedges[3]; // 对应mDEdges数组
	};

	class VCell
	{
	public:
		int itp; // 对应 mTriPoints
		Vec3 tp; // 
		std::vector<int> polygon; // 对应 mVEdgesPoints
		std::vector<VEdge> edges; // 对应 mVEdgesPoints
	};


	typedef std::vector<VEdge> VEdgeArray; // edge container


	typedef std::vector<VCCPoint> VCCPointArray; // triangle's circumcenter point container
	typedef std::vector<VTriPoint> VTriPointArray; // triangle's vertex point container
	typedef std::vector<VTriEdge> VTriEdgeArray; // triangle's edge container
	typedef std::vector<VTriangle> VTriangleArray; // triangle container
	typedef std::vector<VCell> VCellArray; // cell container

	class Voronoi
	{
	public:
		Voronoi();
		~Voronoi();


		void SetData(Delaunay* delaunay);
		
		//std::vector<VEdge> GetEdges(Delaunay* delaunay);

		//int AddVEdge(const VEdge& edge);

		int GetTriEdge(int vi1, int vi2, VTriEdge & edge);
		std::vector<int> GetTrianglesByEdgeIndex(int ei);
		std::vector<int> GetEdgesByPointIndex(int pi);

		VCCPointArray mCCPoints;
		VTriPointArray mTriPoints;
		
		PointArray mVEdgesPoints; // Voronoi Cell图边界点
		//VEdgeArray mVEdges;
		VTriEdgeArray mTriEdges;
		VTriangleArray mTriangles;
		VCellArray mCells;
	};

}


