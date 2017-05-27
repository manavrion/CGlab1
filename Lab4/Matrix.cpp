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

PointF *GMatrix::getProjection(GPointF point, GPointF viewPoint) {
	
	if (point.z + 1 >= viewPoint.z) {
		return nullptr;
	}

	float c = viewPoint.z;

	/*if ((point.z - c) == 0) {
		return nullptr;
	}
	if (c == 0) {
		return nullptr;
	}*/
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

	float param = (-t / (c) + 1);

	for (auto &ob : newpts) {
		ob /= param;
	}




	const float Eps = 175;
	if (abs(newpts[0]) <= Eps && abs(newpts[1]) <= Eps) {
		return new PointF(newpts[0], newpts[1]);
	}	
	return nullptr;
}
