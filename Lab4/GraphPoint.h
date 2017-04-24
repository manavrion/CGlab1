#pragma once
#include "afx.h"
#include "GraphElement.h"

class GraphPoint :
	public GraphElement,
	public Old2DEdition::GraphPoint {
public:
	GraphPoint(float x, float y, float z, std::wstring name = L"") 
		: Old2DEdition::GraphPoint(x, y, z, name) {
		setNewProjection();
	};

	GraphPoint(float x, float y, float z, Color color, std::wstring name = L"") 
		: Old2DEdition::GraphPoint(x, y, z, name), GraphElement(color)
	{
		setNewProjection();
	};

	void setNewProjection() {
		static bool fl = true;
		if (fl) {
			fl = false;
		} else {
			return;
		}
		Old2DEdition::GraphPoint::setNewProjFunc([](float x, float y, float z, PointF center)->PointF {
			center.X += x;
			center.Y += -y;

			center.X -= z*0.75;
			center.Y += z*0.75;
			return center;
		});
	}


	void paintPerspective(Graphics &graphics, PointF center, GPointF viewPoint);

	PointF *getPerspectiveProjectionPoint(PointF center, GPointF viewPoint);


	GPointF getGPointF() {
		return GPointF({x, y, z}); 
	}
};