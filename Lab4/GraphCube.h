#pragma once
#include "GraphElement.h"
#include "GraphLine.h"
#include "Matrix.h"
#include <map>
#include <vector>
#include <algorithm>
#include <set>

struct GraphCube : public GraphElement {

	GraphCube(float center, float lineSz) {
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

		originalPoints = points;
		initLines();
	}
	
	void initLines() {
		for (auto &line : lines) {
			delete line;
		}
		lines.clear();
		plates.clear();
		int width = 1;

		lines.push_back(new GraphLine(points[0], points[1], color, width));
		lines.push_back(new GraphLine(points[7], points[1], color, width));
		lines.push_back(new GraphLine(points[7], points[2], color, width));
		lines.push_back(new GraphLine(points[2], points[0], color, width));

		plates.push_back(vector<GraphLine*>(lines.begin(), lines.begin() + 4));



		lines.push_back(new GraphLine(points[0], points[3], color, width));
		lines.push_back(new GraphLine(points[3], points[6], color, width));
		lines.push_back(new GraphLine(points[6], points[1], color, width));
		//new GraphLine(points[1], points[0], color, width)

		plates.push_back(vector<GraphLine*>(lines.begin() + 4, lines.begin() + 4 + 3));
		plates[1].push_back(lines[0]);



		//new GraphLine(points[0], points[2], color, width)
		lines.push_back(new GraphLine(points[2], points[5], color, width));
		lines.push_back(new GraphLine(points[5], points[3], color, width));
		//lines.push_back(new GraphLine(points[3], points[0], color, width));

		plates.push_back(vector<GraphLine*>(lines.begin() + 4 + 3, lines.begin() + 4 + 3 + 2));
		plates[2].push_back(lines[3]);
		plates[2].push_back(lines[4]);



		lines.push_back(new GraphLine(points[4], points[5], color, width));
		//lines.push_back(new GraphLine(points[2], points[5], color, width));
		//lines.push_back(new GraphLine(points[7], points[2], color, width));
		lines.push_back(new GraphLine(points[4], points[7], color, width));

		plates.push_back(vector<GraphLine*>(lines.begin() + 4 + 3 + 2, lines.begin() + 4 + 3 + 2 + 2));
		plates[3].push_back(lines[7]);
		plates[3].push_back(lines[2]);


		//lines.push_back(new GraphLine(points[4], points[7], color, width));
		lines.push_back(new GraphLine(points[4], points[6], color, width));
		lines.push_back(new GraphLine(points[6], points[1], color, width));
		lines.push_back(new GraphLine(points[7], points[1], color, width));

		plates.push_back(vector<GraphLine*>(lines.begin() + 4 + 3 + 2 + 2, lines.begin() + 4 + 3 + 2 + 2 + 3));
		plates[4].push_back(lines[10]);
		
		//lines.push_back(new GraphLine(points[4], points[6], color, width));
		//lines.push_back(new GraphLine(points[3], points[6], color, width));
		//lines.push_back(new GraphLine(points[5], points[3], color, width));
		//lines.push_back(new GraphLine(points[4], points[5], color, width));
		plates.push_back(vector<GraphLine*>());

		plates[5].push_back(lines[11]);
		plates[5].push_back(lines[5]);
		plates[5].push_back(lines[8]);
		plates[5].push_back(lines[9]);

		/*lines.push_back(new GraphLine(points[0], points[1], color, width));
		lines.push_back(new GraphLine(points[0], points[2], color, width));
		lines.push_back(new GraphLine(points[0], points[3], color, width));

		lines.push_back(new GraphLine(points[4], points[5], color, width));
		lines.push_back(new GraphLine(points[4], points[6], color, width));
		lines.push_back(new GraphLine(points[4], points[7], color, width));

		lines.push_back(new GraphLine(points[7], points[1], color, width));
		lines.push_back(new GraphLine(points[1], points[6], color, width));
		lines.push_back(new GraphLine(points[6], points[3], color, width));
		lines.push_back(new GraphLine(points[3], points[5], color, width));
		lines.push_back(new GraphLine(points[5], points[2], color, width));
		lines.push_back(new GraphLine(points[2], points[7], color, width));*/
	}

	vector<vector<GraphLine*>> plates;

	
	vector<GraphPoint> originalPoints;
	vector<GraphPoint> points;

	vector<GraphLine*> lines;

	void paint(Graphics &graphics, PointF center) {
		if (!visible) return;

		for (auto &ob : lines) {
			ob->paint(graphics, center);
		}

	};

	void paintComplex(Graphics &graphics, PointF center) {
		if (!visible) return;
		
		for (auto &ob : lines) {
			ob->paintComplex(graphics, center);
		}
	};


	map<GraphLine*, bool> isvis;
	void paintPerspective(Graphics &graphics, PointF center, GPointF viewPoint) {

		if (!visible) return;		

		updateVisInfo(viewPoint);

		for (auto &ob : lines) {
			if (isvis[ob]) {
				ob->setColor(color);
			} else {
				ob->setColor(Gdiplus::Color(100, color.GetR(), color.GetG(), color.GetB()));
			}
			ob->paintPerspective(graphics, center, viewPoint);
		}
	};

	virtual ~GraphCube() {
		originalPoints.clear();
		points.clear();
		for (auto &ob : lines) {
			delete ob;
		}
	}

	void updateVisInfo(GPointF viewPoint) {

		

	}





	void resetCube() {
		points = originalPoints;
		initLines();
	}

	void shiftTo(GPointF m, GPointF n) {
		GPointF px(n.x - m.x, n.y - m.y, n.z - m.z);
		float mat[4][4] = { { 1, 0, 0, -px.x },
						    { 0, 1, 0, -px.y },
						    { 0, 0, 1, -px.z },
						    { 0, 0, 0, 1    } };

		GMatrix matrix(mat);

		for (auto &ob : points) {
			GPointF p = GMatrix::applyMatrixTo(ob, matrix);
			ob.x = p.x;
			ob.y = p.y;
			ob.z = p.z;
		}
		//initLines();
	}

	void rotateTo(GPointF m, GPointF n) {
		//GPointF px(n.x - m.x, n.y - m.y, n.z - m.z);
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