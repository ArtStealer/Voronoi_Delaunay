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
		int vi1, vi2; // ��ӦmCCPoints����
	};


	//circumcenter point
	class VCCPoint
	{
	public:
		Vec3 p;
		int itri; //���������ε����� ��ӦmTriangles

	};

	class VTriPoint
	{
	public:
		Vec3 v;
		std::vector<int> edges; // ��Ӧ mTriEdges
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

		int vi1, vi2; // ��ӦmTriPoints����
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
		int vi[3]; // ��Ӧ mTriPoints
		int dedges[3]; // ��ӦmDEdges����
	};

	class VCell
	{
	public:
		int itp; // ��Ӧ mTriPoints
		Vec3 tp; // 
		std::vector<int> polygon; // ��Ӧ mVEdgesPoints
		std::vector<VEdge> edges; // ��Ӧ mVEdgesPoints
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
		
		PointArray mVEdgesPoints; // Voronoi Cellͼ�߽��
		//VEdgeArray mVEdges;
		VTriEdgeArray mTriEdges;
		VTriangleArray mTriangles;
		VCellArray mCells;
	};

}


