#pragma once
#include "GraphLine.h"
#include "GraphWrapPoint.h"

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