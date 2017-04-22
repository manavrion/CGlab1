#pragma once
#include "../CommonFiles/GraphLine.h"
#include "../CommonFiles/GraphWrapCube.h"

struct GraphWrapLine : public GraphElement {

	GraphWrapLine(GraphLine &line, Color color, int width = 2) :
		line(line), a(line.a, color, width), b(line.b, color, width)
	{
		line.setVisible(false);
	};
	
	GraphLine &line;
	GraphWrapCube a;
	GraphWrapCube b;

	void paint(Graphics &graphics, PointF center) {
		PointF posA = a.target.getProjection(center);
		PointF posB = b.target.getProjection(center);
		paintBresenhamLine(graphics, posA.X, posB.X, posA.Y, posB.Y);

		if (!visible) return;
		a.paint(graphics, center);
		b.paint(graphics, center);			

		paintBresenhamLine(graphics, a.target.getProjectionPointXY(center), b.target.getProjectionPointXY(center), color);
		paintBresenhamLine(graphics, a.target.getProjectionPointXZ(center), b.target.getProjectionPointXZ(center), color);
		paintBresenhamLine(graphics, a.target.getProjectionPointYZ(center), b.target.getProjectionPointYZ(center), color);
			
	};

	void paintComplex(Graphics &graphics, PointF center) {
		paintBresenhamLine(graphics, line.a.getComplexProjection1(center), line.b.getComplexProjection1(center));
		paintBresenhamLine(graphics, line.a.getComplexProjection2(center), line.b.getComplexProjection2(center));
		paintBresenhamLine(graphics, line.a.getComplexProjection3(center), line.b.getComplexProjection3(center));

		if (!visible) return;
		a.paintComplex(graphics, center);
		b.paintComplex(graphics, center);
		
	};

	void paintPerspective(Graphics &graphics, PointF center) {};
	
	void paintBresenhamLine(Graphics &graphics, PointF a, PointF b, Color color = Color::White) {
		paintBresenhamLine(graphics, a.X, b.X, a.Y, b.Y, color);
	}

	void paintBresenhamLine(Graphics &graphics, int x1, int x2, int y1, int y2, Color color = Color::White) {
		SolidBrush brush(color);

		function<void(int, int)> setPixel = [&](int x, int y) {
			graphics.SetSmoothingMode(SmoothingMode::SmoothingModeNone);
			graphics.FillRectangle(&brush, Rect(x, y, 1, 1));
			graphics.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);
		};


		const int deltaX = abs(x2 - x1);
		const int deltaY = abs(y2 - y1);
		const int signX = x1 < x2 ? 1 : -1;
		const int signY = y1 < y2 ? 1 : -1;
		//
		int error = deltaX - deltaY;
		//
		setPixel(x2, y2);
		while (x1 != x2 || y1 != y2) {
			setPixel(x1, y1);
			const int error2 = error * 2;
			//
			if (error2 > -deltaY) {
				error -= deltaY;
				x1 += signX;
			}
			if (error2 < deltaX) {
				error += deltaX;
				y1 += signY;
			}
		}

	}
};