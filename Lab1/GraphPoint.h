#pragma once
#include "GraphElement.h"

struct GraphPoint : public GraphElement {

	GraphPoint(float x, float y, float z) : x(x), y(y), z(z), GraphElement(Color::White) {};

	GraphPoint(float x, float y, float z, Color color) : x(x), y(y), z(z), GraphElement(color) {};

	float x, y, z;

	PointF getProjection(PointF center) {
		center.X += -x;
		center.Y += -z;

		center.X += y*0.75;
		center.Y += y*0.75;
		return center;
	}
	
	void paint(Graphics &graphics, PointF center) {
		PointF pos = getProjection(center);
		graphics.FillEllipse(&SolidBrush(color), RectF(PointF(pos.X - 3, pos.Y - 3), SizeF(6, 6)));
	};

	void paintComplex(Graphics &graphics, PointF center) {
		center.X -= 3;
		center.Y -= 3;

		SolidBrush brush(color);

		graphics.FillEllipse(&brush, RectF(PointF(center.X - x, center.Y + y), SizeF(6, 6)));
		graphics.FillEllipse(&brush, RectF(PointF(center.X - x, center.Y - z), SizeF(6, 6)));
		graphics.FillEllipse(&brush, RectF(PointF(center.X + y, center.Y - z), SizeF(6, 6)));
	};

};