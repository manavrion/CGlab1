#pragma once
#include "afx.h"
#include "GraphElement.h"
#include "GraphPoint.h"
#include "Matrix.h"

struct GraphLine : public GraphElement, public Old2DEdition::GraphLine {

	GraphLine(GraphPoint &a, GraphPoint &b, Color color, int width = 2) 
		: Old2DEdition::GraphLine(a, b, color, width)
	{
		this->color = color;
	}

	void paintPerspective(Graphics &graphics, PointF center, GPointF viewPoint) {

		if (!visible) return;
		PointF *pos0 = toCenter(GMatrix::getProjection(this->a, viewPoint), center);
		PointF *pos1 = toCenter(searchPoint(viewPoint), center);
		PointF *pos2 = toCenter(GMatrix::getProjection(this->b, viewPoint), center);

		if (pos1 == nullptr) {
			drawLine(graphics, pos0, pos2);
		} else {
			drawLine(graphics, pos0, pos1);
			drawLine(graphics, pos1, pos2);
		}		

		delete pos0;
		delete pos1;
		delete pos2;
	};

protected:

	PointF *searchPoint(GPointF viewPoint) {

		const float eps = 0.001;

		GPointF l = this->a;
		GPointF r = this->b;

		PointF *fl = GMatrix::getProjection(l, viewPoint);
		PointF *fr = GMatrix::getProjection(r, viewPoint);

		if (fl == fr) {
			delete fl;
			delete fr;
			return nullptr;
		}
		if (fl != nullptr) {
			swap(fl, fr);
			swap(l, r);
		}

		//binary search
		while (l.getGistanceTo(r) > eps) {

			GPointF h((l.x + r.x)/2.0, (l.y + r.y)/2.0, (l.z + r.z)/2.0);

			PointF *fh = GMatrix::getProjection(h, viewPoint);

			if (fh == nullptr) {
				l = h;
			} else {
				r = h;
			}
			delete fh;
		}
		delete fl;
		delete fr;

		return GMatrix::getProjection(r, viewPoint);
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