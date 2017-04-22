#pragma once
#include "GraphPoint.h"
#include <vector>


struct GraphTriangle : public GraphElement {

	GraphTriangle(GraphPoint &a, GraphPoint &b, GraphPoint &c, Color color, int width = 2) 
		: a(a), b(b), c(c), 
		GraphElement(color), width(width) 
	{ }

	GraphPoint &a;
	GraphPoint &b;
	GraphPoint &c;
	int width;

	void paint(Graphics &graphics, PointF center) {
		if (!visible) return;

		drawTriangle(graphics, a.getProjection(center), b.getProjection(center), c.getProjection(center));
	}

	
	void paintComplex(Graphics &graphics, PointF center) {
		if (!visible) return;

		drawTriangle(graphics, a.getComplexProjection1(center), b.getComplexProjection1(center), c.getComplexProjection1(center));
		drawTriangle(graphics, a.getComplexProjection2(center), b.getComplexProjection2(center), c.getComplexProjection2(center));
		drawTriangle(graphics, a.getComplexProjection3(center), b.getComplexProjection3(center), c.getComplexProjection3(center));		
	}

protected:
	void drawTriangle(Graphics &graphics, PointF posA, PointF posB, PointF posC) {
		graphics.DrawLine(&Pen(color, width), posA, posB);
		graphics.DrawLine(&Pen(color, width), posB, posC);
		graphics.DrawLine(&Pen(color, width), posC, posA);
	}

};



/*
struct GraphWrapLine : public GraphElement {

	GraphWrapLine(bool &showLabels, GraphLine &line, Color color, int width = 2) :
		a(showLabels, line.a, color, L"A"), b(showLabels, line.b, color, L"B"), GraphElement(color), line(line),
		projColor(Color(150, 150, 150, 150)), brush(Color(255, 150, 150, 150)), pen(projColor, 1) {};
	
	GraphLine &line;
	GraphWrapCube a;
	GraphWrapCube b;

	Color projColor;

	SolidBrush brush;

	Pen pen;

	void paintPoint(Graphics &graphics, PointF point) {
		point.X -= 3;
		point.Y -= 3;
		graphics.FillEllipse(&brush, RectF(point, SizeF(6, 6)));

	}

	void paint(Graphics &graphics, PointF center) {

		a.paint(graphics, center);
		b.paint(graphics, center);			

		//graphics.DrawLine(&pen, a.getProjectionPointXY(center), b.getProjectionPointXY(center));
		//graphics.DrawLine(&pen, a.getProjectionPointXZ(center), b.getProjectionPointXZ(center));
		//graphics.DrawLine(&pen, a.getProjectionPointYZ(center), b.getProjectionPointYZ(center));

		paintBresenhamLine(graphics, a.getProjectionPointXY(center), b.getProjectionPointXY(center), projColor);
		paintBresenhamLine(graphics, a.getProjectionPointXZ(center), b.getProjectionPointXZ(center), projColor);
		paintBresenhamLine(graphics, a.getProjectionPointYZ(center), b.getProjectionPointYZ(center), projColor);

		paintPoint(graphics, a.getProjectionPointXY(center));
		paintPoint(graphics, a.getProjectionPointXZ(center));
		paintPoint(graphics, a.getProjectionPointYZ(center));

		paintPoint(graphics, b.getProjectionPointXY(center));
		paintPoint(graphics, b.getProjectionPointXZ(center));
		paintPoint(graphics, b.getProjectionPointYZ(center));

		PointF posA = line.a.getProjection(center);
		PointF posB = line.b.getProjection(center);
		paintBresenhamLine(graphics, posA.X, posB.X, posA.Y, posB.Y);
	};

	void paintComplex(Graphics &graphics, PointF center) {

		a.paintComplex(graphics, center);
		b.paintComplex(graphics, center);


		graphics.DrawLine(&pen, a.getPnt1(center), b.getPnt1(center));
		graphics.DrawLine(&pen, a.getPnt2(center), b.getPnt2(center));
		graphics.DrawLine(&pen, a.getPnt3(center), b.getPnt3(center));

		paintPoint(graphics, a.getPnt1(center));
		paintPoint(graphics, a.getPnt2(center));
		paintPoint(graphics, a.getPnt3(center));

		paintPoint(graphics, b.getPnt1(center));
		paintPoint(graphics, b.getPnt2(center));
		paintPoint(graphics, b.getPnt3(center));

	};
};
*/