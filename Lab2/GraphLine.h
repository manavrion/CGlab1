#pragma once

#include "GraphElement.h"
#include "GraphPoint.h"

struct GraphLine : public GraphElement {

	GraphLine(GraphPoint a, GraphPoint b, Color color, int width = 2) : a(a), b(b), width(width), GraphElement(color) {};

	GraphPoint a;
	GraphPoint b;

	int width;

	void paint(Graphics &graphics, PointF center) {
		PointF posA = a.getProjection(center);
		PointF posB = b.getProjection(center);
		graphics.DrawLine(&Pen(color, width), posA, posB);
	};

	void paintComplex(Graphics &graphics, PointF center) {};
};