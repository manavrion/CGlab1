#pragma once
#include "afx.h"
#include "GraphPoint.h"

struct GraphWrapCube : public GraphElement, public Old2DEdition::GraphWrapCube {

	GraphWrapCube(GraphPoint &target, Color color, int width = 2) :
		Old2DEdition::GraphWrapCube(target, color, width) {};

	void paintPerspective(Graphics &graphics, PointF center, GPointF viewPoint) {};
};
