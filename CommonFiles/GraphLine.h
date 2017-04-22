#pragma once
#include "afx.h"
#include "GraphElement.h"
#include "GraphPoint.h"

struct GraphLine : public GraphElement {

	GraphLine(GraphPoint &a, GraphPoint &b, Color color, int width = 2) 
		: a(a), b(b), 
		GraphElement(color), width(width)
	{ }

	GraphPoint &a;
	GraphPoint &b;
	int width;

	void paint(Graphics &graphics, PointF center) {
		if (!visible) return;
		PointF posA = a.getProjection(center);
		PointF posB = b.getProjection(center);
		graphics.DrawLine(&Pen(color, width), posA, posB);
	};

	void paintComplex(Graphics &graphics, PointF center) {
		if (!visible) return;
		graphics.DrawLine(&Pen(color, width), a.getComplexProjection1(center), b.getComplexProjection1(center));
		graphics.DrawLine(&Pen(color, width), a.getComplexProjection2(center), b.getComplexProjection2(center));
		graphics.DrawLine(&Pen(color, width), a.getComplexProjection3(center), b.getComplexProjection3(center));		
	};

	void paintPerspective(Graphics &graphics, PointF center) {};
};