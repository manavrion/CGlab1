#pragma once
#include "afx.h"
#include "GraphPoint.h"
#include "GraphLine.h"

struct GraphXYZ : public GraphElement {

	GraphXYZ(Color xyzColor, Color textColor) :
		GraphElement(xyzColor),
		textBrush(textColor),
		font(L"Arial", 12, FontStyleBold) {

	}

	void init() {
		int xyzHeight = 130;

		//xyz
		toPaintElements.push_back(new GraphLine(*new GraphPoint(xyzHeight, 0, 0), *new GraphPoint(-xyzHeight, 0, 0), color));
		toPaintElements.push_back(new GraphLine(*new GraphPoint(0, xyzHeight, 0), *new GraphPoint(0, -xyzHeight, 0), color));
		toPaintElements.push_back(new GraphLine(*new GraphPoint(0, 0, xyzHeight), *new GraphPoint(0, 0, -xyzHeight), color));

		//xyz arrows
		/*
		toPaintElements.push_back(new GraphLine(*new GraphPoint(xyzHeight, 0, 0), *new GraphPoint(xyzHeight - 20, 0, 10), color));
		toPaintElements.push_back(new GraphLine(*new GraphPoint(xyzHeight, 0, 0), *new GraphPoint(xyzHeight - 20, 0, -10), color));

		toPaintElements.push_back(new GraphLine(*new GraphPoint(0, xyzHeight, 0), *new GraphPoint(0, xyzHeight - 20, 10), color));
		toPaintElements.push_back(new GraphLine(*new GraphPoint(0, xyzHeight, 0), *new GraphPoint(0, xyzHeight - 20, -10), color));

		toPaintElements.push_back(new GraphLine(*new GraphPoint(0, 0, xyzHeight), *new GraphPoint(10, 0, xyzHeight - 20), color));
		toPaintElements.push_back(new GraphLine(*new GraphPoint(0, 0, xyzHeight), *new GraphPoint(-10, 0, xyzHeight - 20), color));*/
	}

	vector<GraphLine*> toPaintElements;
	Font font;
	SolidBrush textBrush;

	///
	virtual void paint(Graphics &graphics, PointF center_p) {
		if (!visible) return;

		for (auto &ob : toPaintElements) {
			ob->paint(graphics, center_p);
		}

		int center = center_p.X;

		// painting labels
		graphics.DrawString(L"Y", -1, &font, PointF(center + 10, center / 4), &textBrush);
		graphics.DrawString(L"X", -1, &font, PointF(center * 1.75, center + 9), &textBrush);
		graphics.DrawString(L"Z", -1, &font, PointF(center / 2, center * 2 - center / 2 + 9), &textBrush);
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

	virtual ~GraphXYZ() {
		for (auto &ob : toPaintElements) {
			delete &(ob->a);
			delete &(ob->b);
			delete ob;
		}
	}

};



