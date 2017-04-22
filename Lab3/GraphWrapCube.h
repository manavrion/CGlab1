#pragma once

#include "GraphPoint.h"

struct GraphWrapCube : public GraphElement {

	GraphWrapCube(GraphPoint &target, Color color, int width = 2) :
		target(target), width(width), GraphElement(color) {};

	GraphPoint &target;
	int width;

	void paint(Graphics &graphics, PointF center) {
		if (!visible) return;

		PointF point_x = target.getProjectionPointX(center);
		PointF point_y = target.getProjectionPointY(center);
		PointF point_z = target.getProjectionPointZ(center);			

		PointF point_xy = target.getProjectionPointXY(center);
		PointF point_xz = target.getProjectionPointXZ(center);
		PointF point_yz = target.getProjectionPointYZ(center);

		Pen pen(color, width);

		graphics.DrawLine(&pen, point_x, center);
		graphics.DrawLine(&pen, point_y, center);
		graphics.DrawLine(&pen, point_z, center);
		
		graphics.DrawLine(&pen, point_x, point_xy);
		graphics.DrawLine(&pen, point_y, point_xy);

		graphics.DrawLine(&pen, point_x, point_xz);
		graphics.DrawLine(&pen, point_z, point_xz);

		graphics.DrawLine(&pen, point_y, point_yz);
		graphics.DrawLine(&pen, point_z, point_yz);

		graphics.DrawLine(&pen, target.getProjection(center), point_xy);
		graphics.DrawLine(&pen, target.getProjection(center), point_xz);
		graphics.DrawLine(&pen, target.getProjection(center), point_yz);

	};

	void paintComplex(Graphics &graphics, PointF center) {
		if (!visible) return;

		PointF pnt1 = target.getComplexProjection1(center);
		PointF pnt2 = target.getComplexProjection2(center);
		PointF pnt3 = target.getComplexProjection3(center);

		Pen pen(color, width);

		graphics.DrawLine(&pen, pnt2, PointF(pnt2.X, center.Y));
		graphics.DrawLine(&pen, pnt2, PointF(center.X, pnt2.Y));

		graphics.DrawLine(&pen, pnt1, PointF(pnt1.X, center.Y));
		graphics.DrawLine(&pen, pnt3, PointF(center.X, pnt3.Y));

		graphics.DrawLine(&pen, pnt1, PointF(center.X, pnt1.Y));
		graphics.DrawLine(&pen, pnt3, PointF(pnt3.X, center.Y));
		
		graphics.DrawBezier(&pen, PointF(center.X, pnt1.Y), PointF(pnt3.X, pnt1.Y), PointF(pnt3.X, center.Y), PointF(pnt3.X, center.Y));
		
	};
};
