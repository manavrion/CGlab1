#pragma once
#include "afx.h"
#include "GraphPoint.h"
#include "GraphLine.h"

namespace OldGraphXYZ {
#include "../CommonFiles/GraphXYZ.h"
}

struct GraphXYZ : public OldGraphXYZ::GraphXYZ {

	GraphXYZ(Color xyzColor, Color textColor) : OldGraphXYZ::GraphXYZ(xyzColor, textColor) { }

	virtual void paint(Graphics &graphics, PointF center_p) {
		if (!visible) return;

		for (auto &ob : toPaintElements) {
			ob->paint(graphics, center_p);
		}

		int center = center_p.X;

		// painting labels
		graphics.DrawString(L"Y", -1, &font, PointF(center + 10, center / 4), &textBrush);
		graphics.DrawString(L"X", -1, &font, PointF(center * 1.75, center + 9), &textBrush);
		graphics.DrawString(L"Z", -1, &font, PointF(center/2, center * 2 - center / 2 + 9), &textBrush);
	};


	void paintPerspective(Graphics &graphics, PointF center, Geometry::GPointF viewPoint) {

		if (!visible) return;

		if (toPaintElements.empty()) {
			init();
		}

		for (auto &ob : toPaintElements) {
			((GraphLine*)ob)->paintPerspective(graphics, center, viewPoint);
		}

		graphics.DrawString(L"Y", -1, &font, PointF(center.X + 10, 20), &textBrush);
		graphics.DrawString(L"X", -1, &font, PointF(center.X * 1.85, center.X + 9), &textBrush);
	};
};

