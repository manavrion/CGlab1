#pragma once
#include "afx.h"
#include "GraphElement.h"
#include "GraphPoint.h"
#include "Matrix.h"

struct GraphLine : public GraphElement {

	GraphLine(GraphPoint &a, GraphPoint &b, Color color, int width = 2)
		: a(a), b(b),
		GraphElement(color), width(width) {}

	GraphPoint &a;
	GraphPoint &b;
	int width;


	void paint(Graphics &graphics, PointF center) {
		if (!visible) return;
		PointF posA = a.getProjection(center);
		PointF posB = b.getProjection(center);
		graphics.DrawLine(&Pen(color, width), posA, posB);
	};

	void paintPerspective(Graphics &graphics, PointF center, Geometry::GPointF viewPoint) {

		if (!visible) return;

		GMatrix::debugProjection(a, viewPoint);
		GMatrix::debugProjection(b, viewPoint);

		PointF *pos0 = toCenter(GMatrix::getProjection(a, viewPoint), center);
		PointF *pos1 = toCenter(searchPoint(a, b, viewPoint), center);
		PointF *pos2 = toCenter(GMatrix::getProjection(b, viewPoint), center);

		if (pos0 != nullptr && pos2 != nullptr) {
			drawLine(graphics, pos0, pos2);
		} else if (pos1 != nullptr && pos0 != nullptr) {
			drawLine(graphics, pos0, pos1);
		} else if (pos1 != nullptr && pos2 != nullptr) {
			drawLine(graphics, pos1, pos2);
		} else {
			delete pos0;
			delete pos1;
			delete pos2;
		}

		delete pos0;
		delete pos1;
		delete pos2;
	};

	operator Geometry::GLine() {
		return Geometry::GLine(a, b);
	}

protected:

	//left -> null
	PointF *searchPoint(Geometry::GPointF left, Geometry::GPointF right, Geometry::GPointF viewPoint) {

		const float eps = 0.001;

		PointF *fl = GMatrix::getProjection(left, viewPoint);
		PointF *fr = GMatrix::getProjection(right, viewPoint);

		if (fl == fr) {
			delete fl;
			delete fr;
			return nullptr;
		}
		if (fl != nullptr) {
			swap(fl, fr);
			swap(left, right);
		}

		//binary search
		while (left.getDistanceTo(right) > eps) {

			Geometry::GPointF h((left.x + right.x)/2.0, (left.y + right.y)/2.0, (left.z + right.z)/2.0);

			PointF *fh = GMatrix::getProjection(h, viewPoint);

			if (fh == nullptr) {
				left = h;
			} else {
				right = h;
			}
			delete fh;
		}
		delete fl;
		delete fr;

		return GMatrix::getProjection(right, viewPoint);
	}
	
	PointF *toCenter(PointF *point, PointF center) {
		if (point == nullptr) {
			return point;
		}
		point->X += center.X;
		point->Y += center.Y;
		return point;
	}

	void drawLine(Graphics &graphics, PointF *a, PointF *b) {
		if (a == nullptr || b == nullptr) {
			return;
		}
		graphics.DrawLine(&Pen(color, width), *a, *b);
	}

};