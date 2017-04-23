#pragma once
#include <cmath>
#include "GPointF.h"

class GMatrix {
public:
	GMatrix() : matrix() {

	};

	GMatrix(float matrix[4][4]) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				this->matrix[i][j] = matrix[i][j];
			}
		}
	};

private:
	float matrix[4][4];

public:
	friend const GMatrix &operator*(const GMatrix &a, const GMatrix &b) {
		GMatrix ret;
		for (int row = 0; row < 3; row++) {
			for (int col = 0; col < 3; col++) {
				for (int inner = 0; inner < 3; inner++) {
					ret.matrix[row][col] += a.matrix[row][inner] * b.matrix[inner][col];
				}
			}
		}
		return ret;
	}


	static GPointF applyMatrixTo(GPointF point, const GMatrix &m);

	static PointF *getProjection(GPointF point, GPointF viewPoint);

public:

	static GMatrix getXRotateMatrix(float a) {
		float matrix[4][4] =
		{ { 1, 0,       0,      0 },
		  { 0, cos(a),  sin(a), 0 },
		  { 0, -sin(a), cos(a), 0 },
		  { 0, 0,       0,      1 }, };
		return GMatrix(matrix);
	}

	static GMatrix getYRotateMatrix(float a) {
		float matrix[4][4] =
		{ { cos(a), 0, -sin(a), 0 },
		  { 0,      1,  0,      0 },
		  { sin(a), 0, cos(a),  0 },
		  { 0, 0,   0,          1 }, };
		return GMatrix(matrix);
	}

	static GMatrix getZRotateMatrix(float a) {
		float matrix[4][4] =
		{ { cos(a),  sin(a), 0, 0 },
		  { -sin(a), cos(a), 0, 0 },
		  { 0,       0,      1, 0 },
		  { 0,       0,      0, 1 }, };
		return GMatrix(matrix);
	}

	static GMatrix getScaleMatrix(float x, float y, float z) {
		float matrix[4][4] =
		{ { x, 0, 0, 0 },
		  { 0, y, 0, 0 },
		  { 0, 0, z, 0 },
		  { 0, 0, 0, 1 }, };
		return GMatrix(matrix);
	}

	static GMatrix getXReflectMatrix(float x, float y, float z) {
		float matrix[4][4] =
		{ { -1, 0, 0, 0 },
		  {  0, 1, 0, 0 },
		  {  0, 0, 1, 0 },
		  {  0, 0, 0, 1 }, };
		return GMatrix(matrix);
	}

	static GMatrix getYReflectMatrix(float x, float y, float z) {
		float matrix[4][4] =
		{ { 1,  0, 0, 0 },
		  { 0, -1, 0, 0 },
		  { 0,  0, 1, 0 },
		  { 0,  0, 0, 1 }, };
		return GMatrix(matrix);
	}

	static GMatrix getZReflectMatrix(float x, float y, float z) {
		float matrix[4][4] =
		{ { 1, 0, 0, 0 },
		  { 0, 1, 0, 0 },
		  { 0, 0, 1, 0 },
		  { 0, 0, 0, -1 }, };
		return GMatrix(matrix);
	}


	GMatrix getToVector(float x, float y, float z) {
		float matrix[4][4] =
		{ { 1, 0, 0, 0 },
		  { 0, 1, 0, 0 },
		  { 0, 0, 1, 0 },
		  { x, y, z, 1 }, };
		return GMatrix(matrix);
	}

public:
	~GMatrix() {

	};
};



