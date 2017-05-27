#pragma once
#include "GraphElement.h"
#include "GraphLine.h"
#include "Matrix.h"
#include <map>
#include <vector>
#include <algorithm>
#include <set>

//прямоугольник в пространстве
struct Plate {
	Plate(GraphLine &a, GraphLine &b, GraphLine &c, GraphLine &d) : a(a), b(b), c(c), d(d) {

	};

	GraphLine a;
	GraphLine b;
	GraphLine c;
	GraphLine d;

};

struct GraphCube : public GraphElement {

	GraphCube(float center, float lineSz) : center(center), lineSz(lineSz) {
		initLines();
	}

protected:
	float center;
	float lineSz;
	
	void initLines() {

		points.clear();
		points.push_back(GraphPoint(lineSz, lineSz, lineSz));
		points.push_back(GraphPoint(0, lineSz, lineSz));
		points.push_back(GraphPoint(lineSz, 0, lineSz));
		points.push_back(GraphPoint(lineSz, lineSz, 0));

		points.push_back(GraphPoint(0, 0, 0));
		points.push_back(GraphPoint(lineSz, 0, 0));
		points.push_back(GraphPoint(0, lineSz, 0));
		points.push_back(GraphPoint(0, 0, lineSz));

		for (auto &ob : points) {
			ob.x -= lineSz / 2 + center;
			ob.y -= lineSz / 2 + center;
			ob.z -= lineSz / 2 + center;
		}

		lines.clear();

		lines.push_back(GraphLine(points[0], points[1], this->color));
		lines.push_back(GraphLine(points[0], points[2], this->color));
		lines.push_back(GraphLine(points[0], points[3], this->color));

		lines.push_back(GraphLine(points[4], points[5], this->color));
		lines.push_back(GraphLine(points[4], points[6], this->color));
		lines.push_back(GraphLine(points[4], points[7], this->color));

		lines.push_back(GraphLine(points[1], points[7], this->color));
		lines.push_back(GraphLine(points[2], points[7], this->color));
		lines.push_back(GraphLine(points[1], points[6], this->color));
		lines.push_back(GraphLine(points[3], points[6], this->color));
		lines.push_back(GraphLine(points[3], points[5], this->color));
		lines.push_back(GraphLine(points[2], points[5], this->color));

		plates.clear();

		plates.push_back(Plate(lines[0], lines[2], lines[9], lines[8]));
		plates.push_back(Plate(lines[1], lines[2], lines[10], lines[11]));
		plates.push_back(Plate(lines[3], lines[5], lines[7], lines[11]));
		plates.push_back(Plate(lines[4], lines[5], lines[6], lines[8]));
		plates.push_back(Plate(lines[0], lines[1], lines[7], lines[6]));
		plates.push_back(Plate(lines[3], lines[4], lines[9], lines[10]));
	}

	vector<GraphPoint> points;
	vector<GraphLine> lines;
	vector<Plate> plates;

	bool isVisibleLine(const GraphLine &line, const GPointF viewPoint) {
		return isVisiblePoint(line.a);
	}

	bool isVisiblePoint(const GraphPoint &point) {



		vector<GPointF> intersects;
		return true;
	}



public:
	void paint(Graphics &graphics, PointF center) {
		if (!visible) return;

		for (auto &ob : lines) {
			ob.paint(graphics, center);
		}

	};

	void paintComplex(Graphics &graphics, PointF center) {
		if (!visible) return;

		for (auto &ob : lines) {
			ob.paintComplex(graphics, center);
		}
	};

	void paintPerspective(Graphics &graphics, PointF center, GPointF viewPoint) {

		if (!visible) return;

		for (auto &ob : lines) {
			if (isVisibleLine(ob, viewPoint)) {
				ob.setColor(color);
			} else {
				ob.setColor(Gdiplus::Color(40, color.GetR(), color.GetG(), color.GetB()));
			}
			ob.paintPerspective(graphics, center, viewPoint);
		}
	};

	void resetCube() {
		initLines();
	}

	virtual ~GraphCube() {
		points.clear();
	}


	void shiftTo(GPointF m, GPointF n) {
		GPointF px(n.x - m.x, n.y - m.y, n.z - m.z);
		float mat[4][4] = { { 1, 0, 0, -px.x },
		{ 0, 1, 0, -px.y },
		{ 0, 0, 1, -px.z },
		{ 0, 0, 0, 1 } };

		GMatrix matrix(mat);

		for (auto &ob : points) {
			GPointF p = GMatrix::applyMatrixTo(ob.getGPointF(), matrix);
			ob.x = p.x;
			ob.y = p.y;
			ob.z = p.z;
		}
	}

	void rotateTo(GPointF m, GPointF n) {
		GPointF px(n.x - m.x, n.y - m.y, n.z - m.z);
		px.normalize();

		GMatrix matrixA2 = GMatrix::getXRotateMatrix(asin(px.y));

		px = GMatrix::applyMatrixTo(px, matrixA2);

		GMatrix matrixA3 = GMatrix::getYRotateMatrix(asin(px.x));

		px = GMatrix::applyMatrixTo(px, matrixA3);

		GMatrix matrix = matrixA2*matrixA3;

		for (auto &ob : points) {
			GPointF p = GMatrix::applyMatrixTo(ob.getGPointF(), matrix);
			ob.x = p.x;
			ob.y = p.y;
			ob.z = p.z;
		}
	}

};