#pragma once
#include "afx.h"

class GraphElement {
public:
	GraphElement(Color color = Color::White) : color(color), visible(true) {}

protected:
	Color color;
	bool visible;

public:
	void setColor(Color color) {
		this->color = color;
	}

	void setVisible(bool visible) {
		this->visible = visible;
	}
    
	Color getColor() {
		return color;
	}

	bool isVisible() {
		return visible;
	}

public:
    virtual void paint(Graphics &graphics, PointF center) = 0;

    virtual void paintComplex(Graphics &graphics, PointF center) = 0;

	virtual void paintPerspective(Graphics &graphics, PointF center) = 0;

};