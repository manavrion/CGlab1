#pragma once
#include "GraphLine.h"
#include <vector>

struct GraphXYZ : public GraphElement {

	GraphXYZ(Color xyzColor, Color textColor) : 
		GraphElement(xyzColor),
		textBrush(textColor),
		font(L"Arial", 12, FontStyleBold) 
	{
		
		int xyzHeight = 130;

		//xyz
		toPaintElements.push_back(new GraphLine(GraphPoint(xyzHeight, 0, 0), GraphPoint(-xyzHeight, 0, 0), xyzColor));
		toPaintElements.push_back(new GraphLine(GraphPoint(0, xyzHeight, 0), GraphPoint(0, -xyzHeight, 0), xyzColor));
		toPaintElements.push_back(new GraphLine(GraphPoint(0, 0, xyzHeight), GraphPoint(0, 0, -xyzHeight), xyzColor));

		//xyz arrows
		toPaintElements.push_back(new GraphLine(GraphPoint(xyzHeight, 0, 0), GraphPoint(xyzHeight - 20, 0, 10), xyzColor));
		toPaintElements.push_back(new GraphLine(GraphPoint(xyzHeight, 0, 0), GraphPoint(xyzHeight - 20, 0, -10), xyzColor));

		toPaintElements.push_back(new GraphLine(GraphPoint(0, xyzHeight, 0), GraphPoint(0, xyzHeight - 20, 10), xyzColor));
		toPaintElements.push_back(new GraphLine(GraphPoint(0, xyzHeight, 0), GraphPoint(0, xyzHeight - 20, -10), xyzColor));

		toPaintElements.push_back(new GraphLine(GraphPoint(0, 0, xyzHeight), GraphPoint(10, 0, xyzHeight - 20), xyzColor));
		toPaintElements.push_back(new GraphLine(GraphPoint(0, 0, xyzHeight), GraphPoint(-10, 0, xyzHeight - 20), xyzColor));


	}

	vector<GraphElement*> toPaintElements;
	Font font;
	SolidBrush textBrush;

	
	void paint(Graphics &graphics, PointF center_p) {

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

		Color xyzColor(140, 140, 140);
		// vert. and hor. lines
		graphics.DrawLine(&Pen(xyzColor, 2), Point(0, center), Point(center * 2, center));
		graphics.DrawLine(&Pen(xyzColor, 2), Point(center, 0), Point(center, center * 2));
		
	};

};

