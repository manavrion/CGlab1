#pragma once
#include "afx.h"
#include "GraphElement.h"
#include "GraphPoint.h"

struct GraphLabel : public GraphElement, public Old2DEdition::GraphLabel {

	GraphLabel(GraphPoint &target, Color color) :
		Old2DEdition::GraphLabel(target, color)
	{ }

	void paintPerspective(Graphics &graphics, PointF center, GPointF viewPoint) {};
};
