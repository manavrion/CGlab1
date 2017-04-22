#pragma once
#include "afx.h"
#include "GraphElement.h"
#include "GraphPoint.h"

struct GraphLabel : public GraphElement {

	GraphLabel(GraphPoint &target, Color color) :
		target(target),
		font(L"Arial", 10, FontStyleBold), 
		textBrush(Color::Yellow),
		border(5)
	{ }

	GraphPoint &target;

	Font font;
	SolidBrush textBrush;
	int border;

	void paint(Graphics &graphics, PointF center) {
		if (!visible) return;

		PointF point_xy = target.getProjectionPointXY(center);
		PointF point_xz = target.getProjectionPointXZ(center);
		PointF point_yz = target.getProjectionPointYZ(center);
		
		graphics.SetTextRenderingHint(TextRenderingHint::TextRenderingHintAntiAlias);

		if (abs(target.getProjection(center).X - center.X) >= border && abs(target.getProjection(center).Y - center.Y) >= border) {
			target.paint(graphics, center);
			graphics.DrawString(target.name.c_str(), -1, &font, PointF(target.getProjection(center).X, target.getProjection(center).Y), &textBrush);
		}

		if (abs(point_xy.X - center.X) >= border && abs(point_xy.Y - center.Y) >= border) {
			paintPointF(graphics, point_xy);
			graphics.DrawString((target.name + L"1").c_str(), -1, &font, PointF(point_xy.X, point_xy.Y), &textBrush);
		}

		if (abs(point_xz.X - center.X) >= border && abs(point_xz.Y - center.Y) >= border) {
			paintPointF(graphics, point_xz);
			graphics.DrawString((target.name + L"2").c_str(), -1, &font, PointF(point_xz.X, point_xz.Y), &textBrush);
		}

		if (abs(point_yz.X - center.X) >= border && abs(point_yz.Y - center.Y) >= border) {
			paintPointF(graphics, point_yz);
			graphics.DrawString((target.name + L"3").c_str(), -1, &font, PointF(point_yz.X, point_yz.Y), &textBrush);
		}
	}

	void paintPointF(Graphics &graphics, PointF pos) {
		graphics.FillEllipse(&SolidBrush(color), RectF(PointF(pos.X - 3, pos.Y - 3), SizeF(6, 6)));
	};

	void paintComplex(Graphics &graphics, PointF center) {
		if (!visible) return;

		PointF pnt1 = target.getComplexProjection1(center);
		PointF pnt2 = target.getComplexProjection2(center);
		PointF pnt3 = target.getComplexProjection3(center);
		
		graphics.SetTextRenderingHint(TextRenderingHint::TextRenderingHintAntiAlias);

		if (abs(pnt1.X - center.X) >= border && abs(pnt1.Y - center.Y) >= border) {
			graphics.FillEllipse(&SolidBrush(color), RectF(PointF(pnt1.X - 3, pnt1.Y - 3), SizeF(6, 6)));
			graphics.DrawString((target.name + L"1").c_str(), -1, &font, PointF(pnt1.X, pnt1.Y), &textBrush);
		}

		if (abs(pnt2.X - center.X) >= border && abs(pnt2.Y - center.Y) >= border) {
			graphics.FillEllipse(&SolidBrush(color), RectF(PointF(pnt2.X - 3, pnt2.Y - 3), SizeF(6, 6)));
			graphics.DrawString((target.name + L"2").c_str(), -1, &font, PointF(pnt2.X, pnt2.Y), &textBrush);
		}

		if (abs(pnt3.X - center.X) >= border && abs(pnt3.Y - center.Y) >= border) {
			graphics.FillEllipse(&SolidBrush(color), RectF(PointF(pnt3.X - 3, pnt3.Y - 3), SizeF(6, 6)));
			graphics.DrawString((target.name + L"3").c_str(), -1, &font, PointF(pnt3.X, pnt3.Y), &textBrush);
		}
	}

	void paintPerspective(Graphics &graphics, PointF center) {};
};
