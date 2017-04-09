#pragma once
#include "stdafx.h"

struct GraphElement {

	GraphElement(Color color) : color(color) {}

	Color color;
    
    virtual void paint(Graphics &graphics, PointF center) = 0;

    virtual void paintComplex(Graphics &graphics, PointF center) = 0;

};