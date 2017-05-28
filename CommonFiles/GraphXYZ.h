#pragma once
#include "afx.h"

struct GraphXYZ : public GraphElement {

	GraphXYZ(Color xyzColor, Color textColor) : 
		GraphElement(color),
		textBrush(textColor),
		font(L"Arial", 12, FontStyleBold) 
	{ }

	virtual void init() {
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

	vector<GraphLine*> toPaintElements;
	Font font;
	SolidBrush textBrush;

	
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
		graphics.DrawString(L"Z", -1, &font, PointF(center + 10, center / 4), &textBrush);
		graphics.DrawString(L"X", -1, &font, PointF(center / 4, center + 9), &textBrush);
		graphics.DrawString(L"Y", -1, &font, PointF(center * 2 - center / 2 + 9, center * 2 - center / 2 + 9), &textBrush);
		
	};

	void paintComplex(Graphics &graphics, PointF center_p) {
		if (!visible) return;

		if (toPaintElements.empty()) {
			init();
		}

		int center = center_p.X;		
		// painting labels
		graphics.DrawString(L"-Y", -1, &font, PointF(0, center + 3), &textBrush);
		graphics.DrawString(L"Y", -1, &font, PointF(center * 2 - 18, center + 3), &textBrush);

		graphics.DrawString(L"X", -1, &font, PointF(3, center - 23), &textBrush);
		graphics.DrawString(L"-X", -1, &font, PointF(center * 2 - 23, center - 23), &textBrush);


		graphics.DrawString(L"-Y", -1, &font, PointF(center - 23, 5), &textBrush);
		graphics.DrawString(L"Z", -1, &font, PointF(center + 5, 5), &textBrush);

		graphics.DrawString(L"-Z", -1, &font, PointF(center - 23, center * 2 - 23), &textBrush);
		graphics.DrawString(L"Y", -1, &font, PointF(center + 5, center * 2 - 23), &textBrush);

		Color xyzColor(0, 0, 0);
		// vert. and hor. lines
		graphics.DrawLine(&Pen(xyzColor, 2), Point(0, center), Point(center * 2, center));
		graphics.DrawLine(&Pen(xyzColor, 2), Point(center, 0), Point(center, center * 2));
		
	};

	virtual ~GraphXYZ() {
		for (auto &ob : toPaintElements) {
			delete &(ob->a);
			delete &(ob->b);
			delete ob;
		}
	}

};

