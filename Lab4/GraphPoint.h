#pragma once
#include "afx.h"
#include "Geometry.h"
#include "Matrix.h"

#include "GraphElement.h"

namespace OldGraphPoint {
#include "../CommonFiles/GraphPoint.h"
}

class GraphPoint : public OldGraphPoint::GraphPoint {
public:
	GraphPoint(float x, float y, float z, std::wstring name = L"") : OldGraphPoint::GraphPoint(x, y, z, name) {};

	GraphPoint(const Geometry::GPointF &pnt) : OldGraphPoint::GraphPoint(pnt.x, pnt.y, pnt.z, L"") {};

	GraphPoint(float x, float y, float z, Color color, std::wstring name = L"") : OldGraphPoint::GraphPoint(x, y, z, color, name) {};

	PointF getProjection(PointF center) {
		center.X += x;
		center.Y += -y;

		center.X -= z*0.75;
		center.Y += z*0.75;
		return center;
	}

	void paintPerspective(Graphics &graphics, PointF center, Geometry::GPointF viewPoint) {
		if (!visible) return;

		center.X -= 3;
		center.Y -= 3;

		SolidBrush brush(color);
		PointF *ret = getPerspectiveProjectionPoint(center, viewPoint);
		if (ret != nullptr) {
			graphics.FillEllipse(&brush, RectF(*ret, SizeF(6, 6)));
			delete ret;
		}
	}

	PointF *getPerspectiveProjectionPoint(PointF center, Geometry::GPointF viewPoint) {
		PointF *ret = GMatrix::getProjection(*this, viewPoint);
		if (ret == nullptr) {
			return ret;
		}
		ret->X += center.X;
		ret->Y += center.Y;
		return ret;
	}

	operator Geometry::GPointF() {
		return Geometry::GPointF(x, y, z);
	}
};