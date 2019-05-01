#include <map>
#include "Voronoi.h"
#include "GeometryUtil.h"

namespace artstealer
{
	Voronoi::Voronoi()
	{
	}

	Voronoi::~Voronoi()
	{
	}

	void Voronoi::SetData(Delaunay * delaunay)
	{
		const DEdgeArray& dedges = delaunay->GetEdges();
		const DTriangleArray& dtris = delaunay->GetTrianges();
		const PointArray& dpoints = delaunay->GetPoints();
		
		mCCPoints.resize(dtris.size());
		mTriangles.resize(dtris.size());
		mTriEdges.resize(dedges.size());
		mTriPoints.resize(dpoints.size());
		
		// ��� mTriPoints,λ��������delaunay->mPoints��ͬ
		for (size_t i = 0; i < dpoints.size(); i++)
		{
			const Vec3& v = dpoints[i];
			VTriPoint p;
			p.v = v;
			mTriPoints[i] = p;

		}

		// ��� mTriEdges
		for (size_t i = 0; i < dedges.size(); i++)
		{
			VTriEdge edge;
			edge.vi1 = dedges[i].vi1;
			edge.vi2 = dedges[i].vi2;
			mTriEdges[i] = edge;
		}

		// ��� mCCPoints, mTriangles
		for (size_t i = 0; i < dtris.size(); i++)
		{
			DTri dtri = dtris[i];
			VCCPoint ccp;
			ccp.p = dtri.cp;
			ccp.itri = i;
			mCCPoints[i] = ccp;
			VTriangle ivtr;
			ivtr.iccp = i;
			ivtr.vi[0] = dtri.vi[0];
			ivtr.vi[1] = dtri.vi[1];
			ivtr.vi[2] = dtri.vi[2];
			VTriEdge _e1, _e2, _e3;
			ivtr.dedges[0] = GetTriEdge(dtri.vi[0], dtri.vi[1], _e1);
			ivtr.dedges[1] = GetTriEdge(dtri.vi[1], dtri.vi[2], _e2);
			ivtr.dedges[2] = GetTriEdge(dtri.vi[0], dtri.vi[2], _e3);
			mTriangles[i] = ivtr;

			mVEdgesPoints.push_back(dtri.cp); // �߽�Ϊ��������
		}
		
		// ���TriEdge�Ĺ���������
		for (size_t i = 0; i < dedges.size(); i++)
		{
			VTriEdge& edge = mTriEdges[i];
			edge.adjTris = GetTrianglesByEdgeIndex(i);
		}

		for (size_t i = 0; i < mTriPoints.size(); i++)
		{
			VTriPoint& point = mTriPoints[i];
			point.edges = GetEdgesByPointIndex(i);
		}

		for (size_t i = 0; i < mTriPoints.size(); i++)
		{
			VCell cell;
			
			const VTriPoint& tp = mTriPoints[i];
			cell.tp = tp.v;
			cell.itp = i;
			int nature = 0;// �����˵㲻�Ƕϵ�
			for (size_t j = 0; j < tp.edges.size(); j++)
			{
				int ivte = tp.edges[j];
				const VTriEdge& vte = mTriEdges[ivte];
				size_t size = vte.adjTris.size();
				if (size == 2)
				{	// ���������γɱ߽�
					int itri1 = vte.adjTris[0];
					int itri2 = vte.adjTris[1];
					const VTriangle& tri1 = mTriangles[itri1];
					const VTriangle& tri2 = mTriangles[itri2];
					const VCCPoint& tri1ccp = mCCPoints[tri1.iccp];
					const VCCPoint& tri2ccp = mCCPoints[tri2.iccp];
					VEdge edge(tri1.iccp, tri2.iccp);
					
					cell.edges.push_back(edge);
					nature = cell.edges.size() - 1;
				}
				else if (size == 1)
				{	// ���� mVEdgesPoints�����ӵ�
					const VTriangle& tri = mTriangles[vte.adjTris[0]];
					const VCCPoint& tri1ccp = mCCPoints[tri.iccp];
					const Vec3& p = tri1ccp.p;
					Vec3 v1 = mTriPoints[vte.vi1].v;
					Vec3 v2 = mTriPoints[vte.vi2].v;
					
					Vec3 e = v2 - v1;
					Vec3 tv1 = mTriPoints[tri.vi[0]].v;
					Vec3 tv2 = mTriPoints[tri.vi[1]].v;
					Vec3 tv3 = mTriPoints[tri.vi[2]].v;

					Vec3 e1 = tv1 - tv2;
					Vec3 e2 = tv2 - tv3;
					Vec3 normal = e1 ^ e2; // ��ֱ�������淨�ߣ�������Ҫ
					Vec3 dir = e ^ normal; // ������ƽ���� ��ֱ�� �� e����
					dir.normalize();
					Vec3 m = (v1 + v2) / 2;
					Vec3 newV = m + dir;

					if (GeometryUtil::IsPointInTriangle(newV, tv1, tv2, tv3)) {
						dir *= -1;
					}
					newV = m + dir * 1000;

					mVEdgesPoints.push_back(newV);
					
					VEdge edge(tri.iccp, mVEdgesPoints.size()-1);
					cell.edges.push_back(edge);
				}
				else
				{
					// error
				}
			}
			if (cell.edges.size() > 0) {
				std::vector<VEdge> temps = cell.edges;
				int vl = cell.edges[nature].vi1;
				int vr = cell.edges[nature].vi2;
				cell.polygon.push_back(vl);
				cell.polygon.push_back(vr);
				temps.erase(temps.begin()+ nature);

				for (size_t k = 0; k < temps.size(); k++)
				{
					if (temps[k].vi1 == vl|| temps[k].vi2 == vl) {
						vl = temps[k].vi1 == vl ? temps[k].vi2 : temps[k].vi1;
						
						cell.polygon.insert(cell.polygon.begin(),vl);
						temps.erase(temps.begin() + k);
						k = -1; // reset
					}
				}
				for (size_t k = 0; k < temps.size(); k++)
				{
					if (temps[k].vi1 == vr || temps[k].vi2 == vr) {
						vr = temps[k].vi1 == vr ? temps[k].vi2 : temps[k].vi1;

						cell.polygon.push_back(vr);
						temps.erase(temps.begin() + k);
						k = -1; // reset
					}
				}
				if (temps.size() > 0) {
					_ASSERT("ERROR");
				}
				//cell.polygon.push_back(cell.edges[0].vi1);
				//std::map<int,bool> 
			}
			
			for (size_t i = 0; i < cell.edges.size(); i++)
			{

			}
			

			mCells.push_back(cell);
		}
		//for (size_t i = 0; i < dtris.size(); i++)
		//{
		//	std::vector<DEdge> neighborEdges;
		//	for (size_t j = 0; j < dtris.size(); j++)
		//	{
		//		if (i != j)
		//		{
		//			DEdge edge = { 0, 0, 0 };
		//			if (delaunay->GetCommonEdge(dtris[i], dtris[j], edge)) {
		//				neighborEdges.push_back(edge);
		//				VEdge ve;
		//				ve.vi1 = i;
		//				ve.vi2 = j;
		//				AddVEdge(ve);
		//			}
		//		}
		//	}
		//}

	}

	//int Voronoi::AddVEdge(const VEdge & edge)
	//{
	//	for (size_t i = 0; i < mVEdges.size(); i++)
	//	{
	//		const VEdge& temp = mVEdges[i];
	//		if (temp.vi1 == edge.vi1 && temp.vi2 == edge.vi2) {
	//			return -1;
	//		}
	//	}
	//	mVEdges.push_back(edge);

	//	return mVEdges.size()-1;
	//}

	int Voronoi::GetTriEdge(int vi1, int vi2, VTriEdge & edge)
	{
		int size = mTriEdges.size();
		for (int i = 0; i < size; i++)
		{
			const VTriEdge& e = mTriEdges[i];
			if (e.vi1 == vi1 && e.vi2 == vi2) {
				edge = e;
				return i;
			}
		}
		return -1;
	}

	std::vector<int> Voronoi::GetTrianglesByEdgeIndex(int ei)
	{
		std::vector<int> tris;
		for (size_t i = 0; i < mTriangles.size(); i++)
		{
			const VTriangle& tri = mTriangles[i];
			if (ei == tri.dedges[0] || ei == tri.dedges[1] || ei == tri.dedges[2]) {
				tris.push_back(i);
			}
		}
		return tris;
	}

	std::vector<int> Voronoi::GetEdgesByPointIndex(int pi)
	{
		std::vector<int> edges;
		for (size_t i = 0; i < mTriEdges.size(); i++)
		{
			const VTriEdge& edge = mTriEdges[i];
			if (edge.vi1 == pi || edge.vi2 == pi)
			{
				edges.push_back(i);
			}
		}
		return edges;
	}

	//std::vector<VEdge> Voronoi::GetEdges(Delaunay * delaunay)
	//{
	//	
	//	std::vector<VEdge> voronoiEdges;
	//	DTriangleArray triangles = delaunay->mTrianges;
	//	DEdgeArray edges = delaunay->mEdges;
	//	PointArray points = delaunay->mPoints;

	//	for (size_t i = 0; i < triangles.size(); i++)
	//	{
	//		std::vector<DEdge> neighborEdges;
	//		for (size_t j = 0; j < triangles.size(); j++)
	//		{
	//			if (i != j)
	//			{
	//				DEdge edge = { 0, 0, 0 };
	//				if (delaunay->GetCommonEdge(triangles[i], triangles[j], edge)) {
	//					neighborEdges.push_back(edge);
	//					VEdge ve = { triangles[i].cp, triangles[j].cp, 0 };

	//				}
	//			}
	//		}
	//	}
	//	List<Edge> voronoiEdgeList = new List<Edge>();
	//	//List<Edge> voronoiRayEdgeList = new List<Edge>();
	//	for (int i = 0; i < allTriangle.Count; i++)
	//	{
	//		List<Edge> neighborEdgeList = new List<Edge>();//�������ڽӱ߼���
	//		for (int j = 0; j < allTriangle.Count; j++)//Ϊ���ҳ��ڽ���������Ϊ2�������Σ�������ߵ������Σ�ѭ��ֻ�ܴ�0��ʼ
	//		{
	//			if (j != i)//��������Ƚ�
	//			{
	//				Edge neighborEdge = findCommonEdge(allTriangle[i], allTriangle[j]);
	//				if (neighborEdge != null)
	//				{
	//					neighborEdgeList.Add(neighborEdge);
	//					//����Voronoi��
	//					Edge voronoiEdge = new Edge(allTriangle[i].centerPoint, allTriangle[j].centerPoint);
	//					if (!voronoiEdgeList.Contains(voronoiEdge))
	//						voronoiEdgeList.Add(voronoiEdge);
	//				}
	//			}
	//		}
	//		if (neighborEdgeList.Count == 2)//��ʾ������������Χ�����Σ�Voronoi����Ҫ����
	//		{
	//			Site midpoint;
	//			Edge rayEdge;
	//			//�ҳ�����߲�Ѱ���е㹹��Voronoi���߱�
	//			if (isPointOnEdge(neighborEdgeList[0], allTriangle[i].site1) && isPointOnEdge(neighborEdgeList[1], allTriangle[i].site1))
	//			{
	//				midpoint = findMidPoint(allTriangle[i].site2, allTriangle[i].site3);
	//				rayEdge = produceRayEdge(allTriangle[i].centerPoint, midpoint);//�����ϳ������ߣ�ԭ��ʵ�ֻ����߶λ�������
	//				voronoiRayEdgeList.Add(rayEdge);
	//			}
	//			if (isPointOnEdge(neighborEdgeList[0], allTriangle[i].site2) && isPointOnEdge(neighborEdgeList[1], allTriangle[i].site2))
	//			{
	//				midpoint = findMidPoint(allTriangle[i].site1, allTriangle[i].site3);
	//				rayEdge = produceRayEdge(allTriangle[i].centerPoint, midpoint);
	//				voronoiRayEdgeList.Add(rayEdge);
	//			}
	//			if (isPointOnEdge(neighborEdgeList[0], allTriangle[i].site3) && isPointOnEdge(neighborEdgeList[1], allTriangle[i].site3))
	//			{
	//				midpoint = findMidPoint(allTriangle[i].site1, allTriangle[i].site2);
	//				rayEdge = produceRayEdge(allTriangle[i].centerPoint, midpoint);
	//				voronoiRayEdgeList.Add(rayEdge);
	//			}
	//		}
	//	}
	//	return std::vector<DEdge>();
	//}

}
