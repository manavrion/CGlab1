#include "stdafx.h"
#include "Matrix.h"

GPointF GMatrix::applyMatrixTo(GPointF point, const GMatrix &m) {
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

/*PointF *GMatrix::getProjection(GPointF point, GPointF viewPoint) {

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

PointF *GMatrix::getProjection(GPointF point, GPointF viewPoint) {

	float focus = 20;

	float scaling = 8;

	GLine main_vector(point, viewPoint);

	Plate screen(
		GPointF(175.0f, 175.0f, viewPoint.z - focus), 
		GPointF(175.0f, -175.0f, viewPoint.z - focus), 
		GPointF(-175.0f, -175.0f, viewPoint.z - focus),
		GPointF(-175.0f, 175.0f, viewPoint.z - focus));

	GPointF *proj = screen.intersectWithLine(main_vector);

	if (proj == nullptr) {
		return nullptr;
	} else {
		PointF *ret = new PointF(proj->x*scaling, proj->y*scaling);
		delete proj;
		return ret;
	}
}