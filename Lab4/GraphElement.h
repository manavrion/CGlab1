#pragma once
#include "afx.h"
#include "GPointF.h"

namespace Old2DEdition {
#include "../CommonFiles/GraphElement.h"
#include "../CommonFiles/GraphPoint.h"
#include "../CommonFiles/GraphLine.h"
#include "../CommonFiles/GraphLabel.h"
#include "../CommonFiles/GraphWrapCube.h"
#include "../CommonFiles/GraphXYZ.h"
}

class GraphElement : virtual public Old2DEdition::GraphElement {
public:
	GraphElement(Color color = Color::White) : Old2DEdition::GraphElement(color) {}

	virtual void paintPerspective(Graphics &graphics, PointF center, GPointF viewPoint) = 0;

};