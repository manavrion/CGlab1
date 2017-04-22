#pragma once

#include "../CommonFiles/GraphElement.h"
#include "../CommonFiles/GraphPoint.h"
#include "GraphTriangle.h"

struct GraphSpecialLine : public GraphElement {

	GraphSpecialLine(GraphPoint &a, GraphPoint &b, GraphTriangle &tr, Color color, int width = 2)
		: a(a), b(b),
		GraphElement(color), width(width), tr(tr), intersectPoint(nullptr) {}

	GraphPoint &a;
	GraphPoint &b;
	GraphTriangle &tr;
	int width;

	GraphPoint *intersectPoint;

	void paint(Graphics &graphics, PointF center) {
		if (!visible) return;

		PointF posA = a.getProjection(center);
		PointF posB = b.getProjection(center);

		Color invisColor(100, color.GetR(), color.GetG(), color.GetB());

		if (intersectPoint == nullptr) {
			
			GraphPoint d((a.x + b.x)/2, (a.y + b.y) / 2, (a.z + b.z) / 2);

			if (isVisible(d)) {				
				graphics.DrawLine(&Pen(color, width), posA, posB);
			} else {
				graphics.DrawLine(&Pen(invisColor, width), posA, posB);
			}			

		} else {

			PointF posC = intersectPoint->getProjection(center);

			if (isVisible(a)) {
				graphics.DrawLine(&Pen(color, width), posC, posA);
			} else {
				graphics.DrawLine(&Pen(invisColor, width), posC, posA);
			}
			if (isVisible(b)) {
				graphics.DrawLine(&Pen(color, width), posC, posB);
			} else {
				graphics.DrawLine(&Pen(invisColor, width), posC, posB);
			}
		}

	};

	void paintComplex(Graphics &graphics, PointF center) {
		if (!visible) return;

		graphics.DrawLine(&Pen(color, width), a.getComplexProjection1(center), b.getComplexProjection1(center));
		graphics.DrawLine(&Pen(color, width), a.getComplexProjection2(center), b.getComplexProjection2(center));
		graphics.DrawLine(&Pen(color, width), a.getComplexProjection3(center), b.getComplexProjection3(center));

	};


	bool isVisible(GraphPoint &pnt) {
		GraphPoint &a = tr.a;
		GraphPoint &b = tr.b;
		GraphPoint &c = tr.c;

		GraphPoint &m = this->a;
		GraphPoint &n = this->b;

		float A = (a.y - b.y)*(a.z + b.z) + (b.y - c.y)*(b.z + c.z) + (c.y - a.y)*(c.z + a.z);
		float B = (a.z - b.z)*(a.x + b.x) + (b.z - c.z)*(b.x + c.x) + (c.z - a.z)*(c.x + a.x);
		float C = (a.x - b.x)*(a.y + b.y) + (b.x - c.x)*(b.y + c.y) + (c.x - a.x)*(c.y + a.y);

		float D = -(A*a.x + B*a.y + C*a.z);
		if (B<0) {
			A = -A;
			B = -B;
			C = -C;
			D = -D;
		}

		if ((A*pnt.x + B*pnt.y + C*pnt.z + D)>0) {
			return true;
		} else if ((A*pnt.x + B*pnt.y + C*pnt.z + D)<0) {
			return false;
		} else {
			return true;
		}

	}

	void paintPerspective(Graphics &graphics, PointF center) {};
};