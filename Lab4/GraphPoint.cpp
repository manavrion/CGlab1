#include "stdafx.h"
#include "afx.h"
#include "GraphPoint.h"
#include "Matrix.h"



void GraphPoint::paintPerspective(Graphics &graphics, PointF center, GPointF viewPoint) {
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

PointF *GraphPoint::getPerspectiveProjectionPoint(PointF center, GPointF viewPoint) {
	PointF *ret = GMatrix::getProjection(*this, viewPoint);
	if (ret == nullptr) {
		return ret;
	}
	ret->X += center.X;
	ret->Y += center.Y;
	return ret;
}
