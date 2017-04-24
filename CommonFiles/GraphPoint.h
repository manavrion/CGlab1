#pragma once
#include "afx.h"
#include "GraphElement.h"
#include <functional>

static function<PointF(float, float, float, PointF)> getProjectionFunc = [](float x, float y, float z, PointF center)->PointF {
	center.X += -x;
	center.Y += -z;

	center.X += y*0.75;
	center.Y += y*0.75;
	return center;
};

class GraphPoint : virtual public GraphElement {
public:
	GraphPoint(float x, float y, float z, std::wstring name = L"") : x(x), y(y), z(z), name(name) {};

	GraphPoint(float x, float y, float z, Color color, std::wstring name = L"") : x(x), y(y), z(z), GraphElement(color), name(name) {};
	
	float x, y, z;
	std::wstring name;


	static void setNewProjFunc(function<PointF(float, float, float, PointF)> f) {
		getProjectionFunc = f;
	}

	PointF getProjection(PointF center) {

		return getProjectionFunc(x, y, z, center);

		//center.X += -x;
		//center.Y += -z;
		//
		//center.X += y*0.75;
		//center.Y += y*0.75;
		//return center;
	}

	PointF getProjectionPointXY(PointF center) {
		return GraphPoint(x, y, 0).getProjection(center);
	}

	PointF getProjectionPointXZ(PointF center) {
		return GraphPoint(x, 0, z).getProjection(center);
	}

	PointF getProjectionPointYZ(PointF center) {
		return GraphPoint(0, y, z).getProjection(center);
	}

	PointF getProjectionPointX(PointF center) {
		return GraphPoint(x, 0, 0).getProjection(center);
	}

	PointF getProjectionPointY(PointF center) {
		return GraphPoint(0, y, 0).getProjection(center);
	}

	PointF getProjectionPointZ(PointF center) {
		return GraphPoint(0, 0, z).getProjection(center);
	}

	PointF getComplexProjection1(PointF center) {
		return PointF(center.X - x, center.Y + y);
	}

	PointF getComplexProjection2(PointF center) {
		return PointF(center.X - x, center.Y - z);
	}

	PointF getComplexProjection3(PointF center) {
		return PointF(center.X + y, center.Y - z);
	}

	void paint(Graphics &graphics, PointF center) {
		if (!visible) return;
		
		PointF pos = getProjection(center);
		graphics.FillEllipse(&SolidBrush(color), RectF(PointF(pos.X - 3, pos.Y - 3), SizeF(6, 6)));
	};

	void paintComplex(Graphics &graphics, PointF center) {
		if (!visible) return;

		center.X -= 3;
		center.Y -= 3;

		SolidBrush brush(color);

		graphics.FillEllipse(&brush, RectF(getComplexProjection1(center), SizeF(6, 6)));
		graphics.FillEllipse(&brush, RectF(getComplexProjection2(center), SizeF(6, 6)));
		graphics.FillEllipse(&brush, RectF(getComplexProjection3(center), SizeF(6, 6)));
	};

};