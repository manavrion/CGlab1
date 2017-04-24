#pragma once
#include "afx.h"
#include "GraphPoint.h"
#include "GraphLine.h"

struct GraphXYZ : public GraphElement, public Old2DEdition::GraphXYZ {

	GraphXYZ(Color xyzColor, Color textColor) : 
		Old2DEdition::GraphXYZ(xyzColor, textColor)
	{ 
		color = xyzColor;
	}

	void init() override {
		
		int xyzHeight = 130;

		//xyz
		toPaintElements.push_back(new GraphLine(*new GraphPoint(xyzHeight, 0, 0), *new GraphPoint(-xyzHeight, 0, 0), color));
		toPaintElements.push_back(new GraphLine(*new GraphPoint(0, xyzHeight, 0), *new GraphPoint(0, -xyzHeight, 0), color));
		toPaintElements.push_back(new GraphLine(*new GraphPoint(0, 0, xyzHeight), *new GraphPoint(0, 0, -xyzHeight), color));

		//xyz arrows
		toPaintElements.push_back(new GraphLine(*new GraphPoint(xyzHeight, 0, 0), *new GraphPoint(xyzHeight - 20, 0, 10), color));
		toPaintElements.push_back(new GraphLine(*new GraphPoint(xyzHeight, 0, 0), *new GraphPoint(xyzHeight - 20, 0, -10), color));
		
		toPaintElements.push_back(new GraphLine(*new GraphPoint(0, xyzHeight, 0), *new GraphPoint(0, xyzHeight - 20, 10), color));
		toPaintElements.push_back(new GraphLine(*new GraphPoint(0, xyzHeight, 0), *new GraphPoint(0, xyzHeight - 20, -10), color));
		
		toPaintElements.push_back(new GraphLine(*new GraphPoint(0, 0, xyzHeight), *new GraphPoint(10, 0, xyzHeight - 20), color));
		toPaintElements.push_back(new GraphLine(*new GraphPoint(0, 0, xyzHeight), *new GraphPoint(-10, 0, xyzHeight - 20), color));
	}

	virtual void paint(Graphics &graphics, PointF center_p) {
		if (!visible) return;

		if (toPaintElements.empty()) {
			init();
		}

		for (auto &ob : toPaintElements) {
			ob->paint(graphics, center_p);
		}

		int center = center_p.X;

		// painting labels
		graphics.DrawString(L"Y", -1, &font, PointF(center + 10, center / 4), &textBrush);
		graphics.DrawString(L"X", -1, &font, PointF(center * 1.75, center + 9), &textBrush);
		graphics.DrawString(L"Z", -1, &font, PointF(center/2, center * 2 - center / 2 + 9), &textBrush);

	};


	void paintPerspective(Graphics &graphics, PointF center, GPointF viewPoint) {

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

