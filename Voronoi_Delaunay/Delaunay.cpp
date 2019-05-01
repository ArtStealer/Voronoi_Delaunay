#include "Delaunay.h"

namespace artstealer
{
	Delaunay::Delaunay(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 v4)
	{
		mPoints.resize(4);
		mPoints[0] = v1;
		mPoints[1] = v2;
		mPoints[2] = v3;
		mPoints[3] = v4;

		
		mEdges.resize(4);
		mEdges[0] = { 0, 1, -1}; // count -1Ϊ�߽�
		mEdges[1] = { 1, 2, -1};
		mEdges[2] = { 0, 3, -1};
		mEdges[3] = { 2, 3, -1};

		AddTriangle(0, 1, 2);
		AddTriangle(0, 2, 3);
		nBorder = 4;
	}

	Delaunay::~Delaunay()
	{
		mPoints.clear();
		mEdges.clear();
		mTrianges.clear();
	}

	void Delaunay::DeleteFrame()
	{
		if (nBorder == 0) {
			return;
		}
		int border = nBorder;
		nBorder = 0;

		DEdgeArray borderEdges;
		mPoints.erase(mPoints.begin(), mPoints.begin()+ border);
		for (size_t i = 0; i < mTrianges.size(); i++)
		{
			if (mTrianges[i].vi[0] <= 3)
			{
				DelTriangle(i, borderEdges);
				borderEdges.clear();
				i--;
			}
			else
			{
				mTrianges[i].vi[0] -= border;
				mTrianges[i].vi[1] -= border;
				mTrianges[i].vi[2] -= border;
			}
		}
		mEdges.erase(mEdges.begin(), mEdges.begin() + border);
		for (size_t i = 0; i < mEdges.size(); i++)
		{
			mEdges[i].vi1 -= 4;
			mEdges[i].vi2 -= 4;
		}

	}

	bool Delaunay::AddPoint(const Vec3 & v)
	{
		DEdgeArray borderEdges;//���ڴ洢��ɾ�������κ����µı߿����ڹ����µ�������  
		//Point newPoint = { xx,yy,zz };
		mPoints.push_back(v);
		

		std::vector<size_t> badTriangles;
		size_t size = mTrianges.size();
		for (size_t i = 0; i < size; i++)
		{
			if (isInCircle(v, mTrianges[i])) 
				badTriangles.push_back(i);
		}
		for (int i = 0; i < (int)badTriangles.size(); i++)
		{
			DelTriangle(badTriangles[i]-i, borderEdges);
		}
		int newVi = (int)mPoints.size()-1;
		for (int i = 0; i < (int)borderEdges.size(); i++)
		{
			int vv[3];
			if (newVi < borderEdges[i].vi1) 
			{ vv[0] = newVi; vv[1] = borderEdges[i].vi1; vv[2] = borderEdges[i].vi2; }
			else if (newVi > borderEdges[i].vi2)
			{ vv[0] = borderEdges[i].vi1; vv[1] = borderEdges[i].vi2; vv[2] = newVi; }
			else
			{ vv[0] = borderEdges[i].vi1; vv[1] = newVi; vv[2] = borderEdges[i].vi2; }

			AddTriangle(vv[0], vv[1], vv[2]);
		}
		return true;
	}

	bool Delaunay::CircumCenter(int vi1, int vi2, int vi3, Vec3& center, REAL & radius)
	{
		Vec3 v1 = mPoints[vi1];
		Vec3 v2 = mPoints[vi2];
		Vec3 v3 = mPoints[vi3];

		double x1, x2, x3, y1, y2, y3;
		x1 = v1.x;
		y1 = v1.y;
		x2 = v2.x;
		y2 = v2.y;
		x3 = v3.x;
		y3 = v3.y;
		REAL x_centre = ((y2 - y1)*(y3 * y3 - y1 * y1 + x3 * x3 - x1 * x1) - (y3 - y1)*(y2 * y2 - y1 * y1 + x2 * x2 - x1 * x1)) / (2 * (x3 - x1)*(y2 - y1) - 2 * ((x2 - x1)*(y3 - y1)));//�������ԲԲ�ĵ�x����  
		REAL y_centre = ((x2 - x1)*(x3 * x3 - x1 * x1 + y3 * y3 - y1 * y1) - (x3 - x1)*(x2 * x2 - x1 * x1 + y2 * y2 - y1 * y1)) / (2 * (y3 - y1)*(x2 - x1) - 2 * ((y2 - y1)*(x3 - x1)));//�������ԲԲ�ĵ�y����  
		radius = sqrt((x1 - x_centre)*(x1 - x_centre) + (y1 - y_centre)*(y1 - y_centre));//�������Բ�İ뾶  
		center.x = x_centre;
		center.y = y_centre;
		return true;
		//Vec3 v1 = mPoints[vi1];
		//Vec3 v2 = mPoints[vi2];
		//Vec3 v3 = mPoints[vi3];

		//Vec3 ac = v3 - v1;
		//Vec3 ab = v2 - v1;

		//// �ж��Ƿ����㹲��
		//Vec3 ac2 = v3 - v1;
		//Vec3 ab2 = v2 - v1;
		//ac2.normalize();
		//ab2.normalize();
		//if (ac2 == ab2)
		//{
		//	_ASSERT("���㹲��");
		//	// ���㹲�ߣ��޷���������
		//	return false;
		//}

		//Vec3 abXac = ab ^ ac;

		//Vec3 abXacXab = abXac ^ ab;
		//Vec3 acXabXac = ac ^ abXac;

		//Vec3 toCenter = (abXacXab * ac.lengthSQ() + acXabXac * ab.lengthSQ()) / (2.f * abXac.lengthSQ());
		//radius = toCenter.length();
		//center = v1 + toCenter;
		//return true;
	}

	bool Delaunay::AddTriangle(int vi1, int vi2, int vi3)
	{
		REAL radius;
		Vec3 center;
		if (!CircumCenter(vi1, vi2, vi3, center, radius)) {
			return false;
		}

		DTri triangle = {{vi1, vi2, vi3}, center, radius };
		mTrianges.push_back(triangle);
		int EdgeSzie = (int)mEdges.size();
		int flag;
		for (int i = 0; i < 3; i++)
		{
			int starti = i;
			int endi = i+1;
			if (i == 2) {
				starti = 0;
				endi = 2;
			}
			flag = 1;
			for (int j = 0; j < EdgeSzie; j++)
			{
				if ((triangle.vi[starti] == mEdges[j].vi1 && triangle.vi[endi] == mEdges[j].vi2))
				{ 
					flag = 0; 
					if (mEdges[j].count != -1)
						mEdges[j].count += 1; 
					break; 
				}
			}
			if (flag == 1) 
				mEdges.push_back({ triangle.vi[starti],triangle.vi[endi],1});
		}
		return true;
	}

	void Delaunay::DelTriangle(int n, DEdgeArray & borderEdges)
	{
		DTri triangle = mTrianges[n];
		for (int i = 0; i < 3; i++)
		{
			int starti = i;
			int endi = i + 1;// ��Ҫ���� vi2 ��ֵ���� vi1��˳�� (i + 1) % 3;
			if (i == 2) {
				starti = 0;
				endi = 2;
			}
			for (int j = 0; j < (int)mEdges.size(); j++)
			{
				if (mEdges[j].vi1 == triangle.vi[starti]&&mEdges[j].vi2 == triangle.vi[endi])
				{
					if (mEdges[j].count == 2)//��Ҫɾ�������ε�һ�ߵļ���Ϊ2�����������1��������ѹ��borderEdges������  
					{
						mEdges[j].count = 1;
						borderEdges.push_back(mEdges[j]);
					}
					else if (mEdges[j].count == -1) //�������߿���ֱ��ѹ��borderEdges������  
					{
						borderEdges.push_back(mEdges[j]);
					}
					else if (mEdges[j].count == 1)//���ɾ�������ε�һ�ߵļ���Ϊ1����ɾ���ñߣ�ͬʱ�鿴borderEdges���Ƿ��д˱ߣ����У���ɾ��  
					{
						for (int k = 0; k < (int)borderEdges.size(); k++)
						{
							if (borderEdges[k].vi1 == mEdges[j].vi1&&borderEdges[k].vi2 == mEdges[j].vi2)
							{
								borderEdges.erase(borderEdges.begin() + k);
								break;
							}
						}
						mEdges.erase(mEdges.begin() + j);
						j--;
					}
					break;
				}
			}
		}
		mTrianges.erase(mTrianges.begin() + n);//ɾ����������  
	}

	bool Delaunay::isInCircle(const Vec3 & p, const DTri & tri)
	{
		double dis = (tri.cp.x - p.x)*(tri.cp.x - p.x) + (tri.cp.y - p.y)*(tri.cp.y - p.y);
		if (dis > tri.radius*tri.radius) 
			return false;
		else 
			return true;
	}

	bool Delaunay::GetCommonEdge(DTri tri1, DTri tri2, DEdge & edge)
	{
		// quick check
		if (tri1.vi[0] >= tri2.vi[2] || tri1.vi[2] <= tri2.vi[0]) {
			return false;
		}
		REAL e[2];
		int count = 0;
		size_t m = 0;
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = m; j < 3; j++)
			{
				if (tri1.vi[i] == tri2.vi[j]) {
					m = j;
					e[count++] = tri1.vi[i];
					break;
				}
			}
		}
		if (count == 0) {
			edge.vi1 = e[0];
			edge.vi2 = e[1];
			return true;
		}
		return false;
	}

	int Delaunay::GetEdge(int vi1, int vi2, DEdge & edge)
	{
		int size = mEdges.size();
		for (int i = 0; i < size; i++)
		{
			const DEdge& e = mEdges[i];
			if (e.vi1 == vi1 && e.vi2 == vi2) {
				edge = e;
				return i;
			}
		}
		return -1;
	}


}

