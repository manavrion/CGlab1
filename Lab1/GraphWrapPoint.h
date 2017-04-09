#pragma once

#include "GraphPoint.h"

struct GraphWrapCube : public GraphElement {

	GraphWrapCube(GraphPoint &target, Color color, int width = 2) : target(target), width(width), GraphElement(color) {};

	GraphPoint &target;
	int width;

	void paint(Graphics &graphics, PointF center) {

		GraphPoint target_x(target.x, 0, 0);
		GraphPoint target_y(0, target.y, 0);
		GraphPoint target_z(0, 0, target.z);

		Pen pen(color, width);

		graphics.DrawLine(&pen, target_x.getProjection(center), center);
		graphics.DrawLine(&pen, target_y.getProjection(center), center);
		graphics.DrawLine(&pen, target_z.getProjection(center), center);

		GraphPoint target_xy(target.x, target.y, 0);
		graphics.DrawLine(&pen, target_x.getProjection(center), target_xy.getProjection(center));
		graphics.DrawLine(&pen, target_y.getProjection(center), target_xy.getProjection(center));

		GraphPoint target_xz(target.x, 0, target.z);
		graphics.DrawLine(&pen, target_x.getProjection(center), target_xz.getProjection(center));
		graphics.DrawLine(&pen, target_z.getProjection(center), target_xz.getProjection(center));

		GraphPoint target_yz(0, target.y, target.z);
		graphics.DrawLine(&pen, target_y.getProjection(center), target_yz.getProjection(center));
		graphics.DrawLine(&pen, target_z.getProjection(center), target_yz.getProjection(center));


		graphics.DrawLine(&pen, target.getProjection(center), target_xy.getProjection(center));
		graphics.DrawLine(&pen, target.getProjection(center), target_xz.getProjection(center));
		graphics.DrawLine(&pen, target.getProjection(center), target_yz.getProjection(center));

	};

	void paintComplex(Graphics &graphics, PointF center) {

		PointF pnt1(center.X - target.x, center.Y + target.y);
		PointF pnt2(center.X - target.x, center.Y - target.z);
		PointF pnt3(center.X + target.y, center.Y - target.z);

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