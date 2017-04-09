#pragma once

#include "GraphPoint.h"

struct GraphWrapCube : public GraphElement {

	GraphWrapCube(bool &showLabels, GraphPoint &target, Color color, wstring pointName = L"A", int width = 2) :
		showLabels(showLabels), target(target), width(width), GraphElement(color),
		font(L"Arial", 10, FontStyleBold), textBrush(Color::Yellow), pointName(pointName) {};

	GraphPoint &target;
	int width;

	Font font;
	SolidBrush textBrush;

	int border = 5;

	wstring pointName;

	bool &showLabels;

	PointF getProjectionPointXY(PointF center) {
		return GraphPoint(target.x, target.y, 0).getProjection(center);
	}

	PointF getProjectionPointXZ(PointF center) {
		return GraphPoint(target.x, 0, target.z).getProjection(center);
	}

	PointF getProjectionPointYZ(PointF center) {
		return GraphPoint(0, target.y, target.z).getProjection(center);
	}

	void paint(Graphics &graphics, PointF center) {

		GraphPoint target_x(target.x, 0, 0);
		GraphPoint target_y(0, target.y, 0);
		GraphPoint target_z(0, 0, target.z);

		Pen pen(color, width);

		GraphPoint target_xy(target.x, target.y, 0);
		GraphPoint target_xz(target.x, 0, target.z);
		GraphPoint target_yz(0, target.y, target.z);

		PointF point_xy = getProjectionPointXY(center);
		PointF point_xz = getProjectionPointXZ(center);
		PointF point_yz = getProjectionPointYZ(center);

		graphics.DrawLine(&pen, target_x.getProjection(center), center);
		graphics.DrawLine(&pen, target_y.getProjection(center), center);
		graphics.DrawLine(&pen, target_z.getProjection(center), center);

		
		graphics.DrawLine(&pen, target_x.getProjection(center), point_xy);
		graphics.DrawLine(&pen, target_y.getProjection(center), point_xy);


		graphics.DrawLine(&pen, target_x.getProjection(center), point_xz);
		graphics.DrawLine(&pen, target_z.getProjection(center), point_xz);


		graphics.DrawLine(&pen, target_y.getProjection(center), point_yz);
		graphics.DrawLine(&pen, target_z.getProjection(center), point_yz);


		graphics.DrawLine(&pen, target.getProjection(center), point_xy);
		graphics.DrawLine(&pen, target.getProjection(center), point_xz);
		graphics.DrawLine(&pen, target.getProjection(center), point_yz);


		target.paint(graphics, center);

		if (showLabels) {
			graphics.SetTextRenderingHint(TextRenderingHint::TextRenderingHintAntiAlias);

			if (abs(target.getProjection(center).X - center.X) >= border && abs(target.getProjection(center).Y - center.Y) >= border) {
				target.paint(graphics, center);
				graphics.DrawString(pointName.c_str(), -1, &font, PointF(target.getProjection(center).X, target.getProjection(center).Y), &textBrush);
			}

			if (abs(point_xy.X - center.X) >= border && abs(point_xy.Y - center.Y) >= border) {
				target_xy.paint(graphics, center);
				graphics.DrawString((pointName + L"1").c_str(), -1, &font, PointF(point_xy.X, point_xy.Y), &textBrush);
			}

			if (abs(point_xz.X - center.X) >= border && abs(point_xz.Y - center.Y) >= border) {
				target_xz.paint(graphics, center);
				graphics.DrawString((pointName + L"2").c_str(), -1, &font, PointF(target_xz.getProjection(center).X, target_xz.getProjection(center).Y), &textBrush);
			}

			if (abs(point_yz.X - center.X) >= border && abs(point_yz.Y - center.Y) >= border) {
				target_yz.paint(graphics, center);
				graphics.DrawString((pointName + L"3").c_str(), -1, &font, PointF(target_yz.getProjection(center).X, point_yz.Y), &textBrush);
			}
		}

		

	};


	PointF getPnt1(PointF center) {
		return PointF(center.X - target.x, center.Y + target.y);
	}

	PointF getPnt2(PointF center) {
		return PointF(center.X - target.x, center.Y - target.z);
	}

	PointF getPnt3(PointF center) {
		return PointF(center.X + target.y, center.Y - target.z);
	}

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

		if (showLabels) {
			graphics.SetTextRenderingHint(TextRenderingHint::TextRenderingHintAntiAlias);

			if (abs(pnt1.X - center.X) >= border && abs(pnt1.Y - center.Y) >= border) {
				graphics.FillEllipse(&SolidBrush(color), RectF(PointF(pnt1.X - 3, pnt1.Y - 3), SizeF(6, 6)));
				graphics.DrawString((pointName + L"1").c_str(), -1, &font, PointF(pnt1.X, pnt1.Y), &textBrush);
			}

			if (abs(pnt2.X - center.X) >= border && abs(pnt2.Y - center.Y) >= border) {
				graphics.FillEllipse(&SolidBrush(color), RectF(PointF(pnt2.X - 3, pnt2.Y - 3), SizeF(6, 6)));
				graphics.DrawString((pointName + L"2").c_str(), -1, &font, PointF(pnt2.X, pnt2.Y), &textBrush);
			}

			if (abs(pnt3.X - center.X) >= border && abs(pnt3.Y - center.Y) >= border) {
				graphics.FillEllipse(&SolidBrush(color), RectF(PointF(pnt3.X - 3, pnt3.Y - 3), SizeF(6, 6)));
				graphics.DrawString((pointName + L"3").c_str(), -1, &font, PointF(pnt3.X, pnt3.Y), &textBrush);
			}

		}
		
	};
};