#pragma once
#include "afx.h"
#include "GraphElement.h"

class GraphPoint :
	public GraphElement,
	public Old2DEdition::GraphPoint {
public:
	GraphPoint(float x, float y, float z, std::wstring name = L"") 
		: Old2DEdition::GraphPoint(x, y, z, name) {};

	GraphPoint(float x, float y, float z, Color color, std::wstring name = L"") 
		: Old2DEdition::GraphPoint(x, y, z, name), GraphElement(color)
	{ };


	void paintPerspective(Graphics &graphics, PointF center, GPointF viewPoint);

	PointF *getPerspectiveProjectionPoint(PointF center, GPointF viewPoint);


	GPointF getGPointF() {
		return GPointF({x, y, z}); 
	}
};