#pragma once
#include "GraphElement.h"
#include "GraphLine.h"
#include "Matrix.h"
#include <map>
#include <vector>
#include <algorithm>
#include <set>

struct GraphCube : public GraphElement {

	GraphCube(float center, float lineSz) : center(center), lineSz(lineSz), debug(false) {
		initLines();
	}

	bool debug;
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

		
	}

	vector<GraphPoint> points;
	vector<GraphLine> lines;

	bool isVisibleLine(const GraphLine &line, const GPointF viewPoint, Graphics &graphics, PointF center) {
		return isVisiblePoint(line.a, viewPoint, graphics, center)
			&& isVisiblePoint(line.b, viewPoint, graphics, center);
	}

	vector<Plate> plates;
	void updatePlates() {
		plates.clear();
		plates.push_back(Plate(points[0], points[1], points[6], points[3]));
		plates.push_back(Plate(points[0], points[2], points[5], points[3]));
		plates.push_back(Plate(points[7], points[2], points[5], points[4]));
		plates.push_back(Plate(points[7], points[1], points[6], points[4]));

		plates.push_back(Plate(points[0], points[1], points[7], points[2]));
		plates.push_back(Plate(points[3], points[6], points[4], points[5]));
	}

	bool isVisiblePoint(const GPointF &point, const GPointF &viewPoint, Graphics &graphics, PointF center) {
		updatePlates();
		GLine mainvect(viewPoint, point);
		
		//mainvect.p1.z += 25;
		//mainvect.p1.x = 0;

		Color d_color(rand() % 255, rand() % 255, rand() % 255);

		if (debug && debug_graphics != nullptr) {
			GraphPoint d_p1(mainvect.p1.x, mainvect.p1.y, mainvect.p1.z);
			GraphPoint d_p2(mainvect.p2.x, mainvect.p2.y, mainvect.p2.z);
			GraphLine debug_v(d_p1, d_p2, d_color, 1);
			debug_v.paint(*debug_graphics, center);
		}
		
		bool ret = true;

		for (auto &plate : plates) {
			GPointF *tmp = plate.intersectWithLine(mainvect);

			if (tmp != nullptr) {
				if (debug && debug_graphics != nullptr) {
					GraphPoint d_p(tmp->x, tmp->y, tmp->z, d_color);
					d_p.paint(*debug_graphics, center);
				}
				delete tmp;
				ret = false;
			}
		}

		return ret;
	}



public:
	Graphics *debug_graphics = nullptr;
	void paint(Graphics &graphics, PointF center) {
		if (!visible) return;

		debug_graphics = &graphics;

		for (auto &ob : lines) {
			ob.paint(graphics, center);
		}
	};

	void paintComplex(Graphics &graphics, PointF center) {	};

	void paintPerspective(Graphics &graphics, PointF center, GPointF viewPoint) {
		
		if (!visible) return;
		updateVisible(viewPoint, graphics, center);

		for (auto &ob : lines) {			
			ob.paintPerspective(graphics, center, viewPoint);
		}
	};

	void updateVisible(GPointF viewPoint, Graphics &graphics, PointF center) {
		srand(666);
		for (auto &ob : lines) {
			if (isVisibleLine(ob, viewPoint, graphics, center)) {
				ob.setColor(color);
			} else {
				ob.setColor(Gdiplus::Color(40, color.GetR(), color.GetG(), color.GetB()));
			}
		}
	}


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
			GPointF p = GMatrix::applyMatrixTo(ob, matrix);
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
			GPointF p = GMatrix::applyMatrixTo(ob, matrix);
			ob.x = p.x;
			ob.y = p.y;
			ob.z = p.z;
		}
	}

};