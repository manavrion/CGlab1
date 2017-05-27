#pragma once
#include "afx.h"
#include "GraphPoint.h"

namespace OldGraphWrapCube {
#include "../CommonFiles/GraphWrapCube.h"
}

struct GraphWrapCube : public OldGraphWrapCube::GraphWrapCube {

	GraphWrapCube(GraphPoint &target, Color color, int width = 2) :
		OldGraphWrapCube::GraphWrapCube(target, color, width) {};

	void paintPerspective(Graphics &graphics, PointF center, GPointF viewPoint) {};
};
