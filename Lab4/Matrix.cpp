#include "stdafx.h"
#include "Matrix.h"
#include "Lab4.h"


#include "GraphLine.h"

Geometry::GPointF GMatrix::applyMatrixTo(Geometry::GPointF point, const GMatrix &m) {
	float one = 1;
	vector<float*> pts;
	vector<float> newpts(4);
	pts.push_back(&(point.x));
	pts.push_back(&(point.y));
	pts.push_back(&(point.z));
	pts.push_back(&one);

	for (int row = 0; row < 4; row++) {
		for (int inner = 0; inner < 4; inner++) {
			newpts[row] += m.matrix[row][inner] * (*pts[inner]);
		}
	}
	for (int i = 0; i < pts.size(); i++) {
		*pts[i] = newpts[i];
	}
	return point;
}

/*PointF *GMatrix::getProjection(Geometry::GPointF point, Geometry::GPointF viewPoint) {

	if (point.z >= viewPoint.z) {
		return nullptr;		
	}

	float c = viewPoint.z;

	float t = -c / (point.z - c);

	float matrix[4][4] =
	{ { 1, 0, 0, 0 },
	{ 0, 1, 0, 0 },
	{ 0, 0, 1, -1.0 / c },
	{ 0, 0, 0, 1 }, };

	

	vector<float> pts;
	vector<float> newpts(4);
	pts.push_back(point.x);
	pts.push_back(point.y);
	pts.push_back(t);
	pts.push_back(1);

	for (int row = 0; row < 4; row++) {
		for (int inner = 0; inner < 4; inner++) {
			newpts[row] += matrix[row][inner] * pts[inner];
		}
	}

	float param = (-t / (c ) + 1);

	if (point.z >= viewPoint.z / param || param == 0 ) {
		return nullptr;
	}

	//if (point.z >= viewPoint.z / param || param == 0 || point.z <= -viewPoint.z / param) {
	//	return nullptr;		
	//}

	for (auto &ob : newpts) {
		ob /= param;
	}
	

	//const float Eps = 1175;
	//if (abs(newpts[0]) <= Eps && abs(newpts[1]) <= Eps) {
	//	return new PointF(newpts[0], newpts[1]);
	//}
	//
	//return nullptr;
	return new PointF(newpts[0], newpts[1]);
}*/


Geometry::GPointF * GMatrix::getIntersect(Geometry::GPointF point, Geometry::GPointF viewPoint) {

	Geometry::GLine main_vector(point, viewPoint);
	Geometry::GPlate screen(viewPoint.z - g_focus); //def g_focus = 20
	Geometry::GPointF *proj = screen.intersectWithLineUnborder(main_vector);

	return proj;
}

PointF *GMatrix::getProjection(Geometry::GPointF point, Geometry::GPointF viewPoint) {

	Geometry::GPointF *proj = getIntersect(point, viewPoint);

	if (proj == nullptr) {
		return nullptr;
	} else {
		PointF *ret = new PointF(proj->x*g_scaling, proj->y*g_scaling); //def g_scaling = 8
		delete proj;
		return ret;
	}
}



void GMatrix::debugProjection(Geometry::GPointF point, Geometry::GPointF viewPoint) {

	if (!g_proj) {
		return;
	}

	Geometry::GPointF *proj = getIntersect(point, viewPoint);

	if (proj == nullptr) {
		return;
	}

	GraphPoint t1 = viewPoint;
	GraphPoint t2 = *proj;

	GraphLine line(t1, t2, Color(85, 150, 150, 255), 1);


	//camera
	GraphPoint &l = t1;
	GraphPoint l1 = Geometry::GPointF(175 / g_scaling, 175 / g_scaling, viewPoint.z - g_focus);
	GraphPoint l2 = Geometry::GPointF(175 / g_scaling, -175 / g_scaling, viewPoint.z - g_focus);
	GraphPoint l3 = Geometry::GPointF(-175 / g_scaling, -175 / g_scaling, viewPoint.z - g_focus);
	GraphPoint l4 = Geometry::GPointF(-175 / g_scaling, 175 / g_scaling, viewPoint.z - g_focus);
	GraphLine line1(l1, l2, Color(5, 255, 0, 0), 1);
	GraphLine line2(l2, l3, Color(5, 255, 0, 0), 1);
	GraphLine line3(l3, l4, Color(5, 255, 0, 0), 1);
	GraphLine line4(l4, l1, Color(5, 255, 0, 0), 1);
	GraphLine line5(l1, l, Color(5, 255, 0, 0), 2);
	GraphLine line6(l2, l, Color(5, 255, 0, 0), 2);
	GraphLine line7(l3, l, Color(5, 255, 0, 0), 2);
	GraphLine line8(l4, l, Color(5, 255, 0, 0), 2);
	line1.paint(*g_firstWindowPlate->getGraphics(), PointF(175, 175));
	line2.paint(*g_firstWindowPlate->getGraphics(), PointF(175, 175));
	line3.paint(*g_firstWindowPlate->getGraphics(), PointF(175, 175));
	line4.paint(*g_firstWindowPlate->getGraphics(), PointF(175, 175));
	line5.paint(*g_firstWindowPlate->getGraphics(), PointF(175, 175));
	line6.paint(*g_firstWindowPlate->getGraphics(), PointF(175, 175));
	line7.paint(*g_firstWindowPlate->getGraphics(), PointF(175, 175));
	line8.paint(*g_firstWindowPlate->getGraphics(), PointF(175, 175));


	if (abs(line.b.x) <= 175 / g_scaling && abs(line.b.y) <= 175 / g_scaling) {
		line.paint(*g_firstWindowPlate->getGraphics(), PointF(175, 175));
	}

}