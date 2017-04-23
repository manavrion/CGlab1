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
	
	if (point.z > viewPoint.z) {
		return nullptr;
	}

	float c = viewPoint.z;

	if ((point.z - c) == 0) {
		return nullptr;
	}
	if (c == 0) {
		return nullptr;
	}
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
	

	//float param = (-t / (c / 32) + 1);
	float param = (-t / (c / 32) + 1);

	if (point.z >= viewPoint.z / param || param == 0 || point.z <= -viewPoint.z / param) {
		return nullptr;
	}

	for (auto &ob : newpts) {
		ob /= param;
	}


	return new PointF(newpts[0], newpts[1]);
}
