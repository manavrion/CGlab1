#pragma once
#include "afx.h"
#include "GraphPoint.h"
#include "GraphLine.h"

struct GraphXYZ : public GraphElement, public Old2DEdition::GraphXYZ {

	GraphXYZ(Color xyzColor, Color textColor) : 
		Old2DEdition::GraphXYZ(xyzColor, textColor)
	{ }


	void paintPerspective(Graphics &graphics, PointF center, GPointF viewPoint) {};
};

