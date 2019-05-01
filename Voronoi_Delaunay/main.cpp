#include <iostream>  
#include <opencv2/opencv.hpp>  
#include <math.h>
#include "Delaunay.h"
#include "Voronoi.h"

using namespace std;
using namespace cv;
using namespace artstealer;

int main(int argc, char** argv)
{
	srand(123456789); //设置随机种子

	int width = 800;
	int height = 600;
	int offsetx = 10;
	int offsety = 10;
	Mat img = Mat::zeros(Size(width + (offsetx<<1), height+ (offsety<<1)), CV_8UC3);

	Delaunay delaunay(Vec3(offsetx, offsety, 0),
		Vec3(width + offsetx, offsety, 0),
		Vec3(width + offsetx, height + offsety, 0),
		Vec3(offsetx, height + offsety, 0)
		);

	Scalar WHITE(255, 255, 255);
	Scalar RED(0, 0, 255);
	Scalar GREEN(0, 255, 0);
	Scalar BLUE(255, 0, 0);
	Scalar YELLOW(0, 255, 255);


	int size = 20;
	for (size_t i = 0; i < size; i++)
	{
		float x = (rand() % width) + offsetx;
		float y = (rand() % height)+ offsety;
		
		delaunay.AddPoint(Vec3(x, y, 0));
	}
	delaunay.DeleteFrame();

	Voronoi voronoi;
	voronoi.SetData(&delaunay);

	const PointArray& points = delaunay.GetPoints();
	const DEdgeArray& edges = delaunay.GetEdges();
	const DTriangleArray& tris = delaunay.GetTrianges();

	for (size_t i = 0; i < tris.size(); i++)
	{
		DTri tri = tris[i];
		Vec3 v1 = points[tri.vi[0]];
		Vec3 v2 = points[tri.vi[1]];
		Vec3 v3 = points[tri.vi[2]];
		Point points[1][3] = { { Point(v1.x,v1.y),Point(v2.x,v2.y),Point(v3.x,v3.y) } };
		
		const Point* ppt[1] = { points[0] };
		int npt[] = { 3 };
		Scalar color = Scalar(rand() % 255, rand() % 255, rand() % 255, 100);
		fillPoly(img, ppt, npt, 1, color);

		color = color * 0.8;
		//circle(img, Point(tri.cp.x, tri.cp.y), 2, color, 2); // 绘制外心
	}


	for (size_t i = 0; i < edges.size(); i++)
	{
		DEdge edge = edges[i];
		Vec3 v1 = points[edge.vi1];
		Vec3 v2 = points[edge.vi2];
		line(img, Point(v1.x, v1.y), Point(v2.x, v2.y), WHITE, 1);
	}

	for (size_t i = 0; i < points.size(); i++)
	{
		Vec3 v = points[i];
		circle(img, Point(v.x, v.y), 2, RED,2);
	}

	VCellArray cells = voronoi.mCells;
	PointArray vepts = voronoi.mVEdgesPoints;
	Mat img2 = Mat::zeros(Size(width + (offsetx << 1), height + (offsety << 1)), CV_8UC3);
	
	for (size_t i = 0; i < cells.size(); i++)
	{
		const VCell& cell = cells[i];
		const Vec3& v = cell.tp;
		

		for (size_t j = 0; j < cell.edges.size(); j++)
		{
			const VEdge& edge = cell.edges[j];
			const Vec3& v1 = vepts[edge.vi1];
			const Vec3& v2 = vepts[edge.vi2];
			line(img2, Point(v1.x, v1.y), Point(v2.x, v2.y), WHITE, 1);
			//line(img, Point(v1.x, v1.y), Point(v2.x, v2.y), YELLOW, 1);
		}

		Point** pts = new Point*[1];
		size_t size = cell.polygon.size();
		pts[0] = new Point[size];
		for (size_t i = 0; i < size; i++)
		{
			const Vec3& vec = vepts[cell.polygon[i]];
			pts[0][i].x = vec.x;
			pts[0][i].y = vec.y;
		}
		const Point* ppt[1] = { pts[0] };
		int npt[] = { size };
		Scalar color = Scalar(rand() % 255, rand() % 255, rand() % 255, 100);
		fillPoly(img2, ppt, npt, 1, color);

		delete[]pts[0];
		delete pts;

		circle(img2, Point(v.x, v.y), 2, RED, 2);
	}

	imshow("Delaunay", img);
	imshow("Voronoi", img2);

	waitKey(0);
	return 0;
}


