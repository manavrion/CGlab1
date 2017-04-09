#pragma once
#include "stdafx.h"

struct GraphElement {

	GraphElement(Color color) : color(color) {}

	Color color;
    
    virtual void paint(Graphics &graphics, PointF center) = 0;

    virtual void paintComplex(Graphics &graphics, PointF center) = 0;

	void paintBresenhamLine(Graphics &graphics, PointF a, PointF b, Color color = Color::White) {
		paintBresenhamLine(graphics, a.X, b.X, a.Y, b.Y, color);
	}
	
	void paintBresenhamLine(Graphics &graphics, int x1, int x2, int y1, int y2, Color color = Color::White) {
		SolidBrush brush(color);

		function<void(int, int)> setPixel = [&](int x, int y) {
			graphics.SetSmoothingMode(SmoothingMode::SmoothingModeNone);			
			graphics.FillRectangle(&brush, Rect(x, y, 1, 1));
			graphics.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);
		};


		const int deltaX = abs(x2 - x1);
		const int deltaY = abs(y2 - y1);
		const int signX = x1 < x2 ? 1 : -1;
		const int signY = y1 < y2 ? 1 : -1;
		//
		int error = deltaX - deltaY;
		//
		setPixel(x2, y2);
		while (x1 != x2 || y1 != y2) {
			setPixel(x1, y1);
			const int error2 = error * 2;
			//
			if (error2 > -deltaY) {
				error -= deltaY;
				x1 += signX;
			}
			if (error2 < deltaX) {
				error += deltaX;
				y1 += signY;
			}
		}




	}

};