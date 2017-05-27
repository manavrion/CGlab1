#include "stdafx.h"
#include "GPointF.h"

GPointF *Plate::intersectWithLine(const GLine & line) {

	GPointF *intersect = GPlane(a, b, c).intersectLineWithTriangle(line);

	if (intersect == nullptr) {
		return nullptr;
	}

	if (a == *intersect || b == *intersect || c == *intersect || d == *intersect) {
		delete intersect;
		return nullptr;
	}


	GLine l1(a, b);
	GLine l2(b, c);
	GLine l3(c, d);
	GLine l4(d, a);

	float p1 = l1.distanceTo(*intersect);
	float p2 = l2.distanceTo(*intersect);
	float p3 = l3.distanceTo(*intersect);
	float p4 = l4.distanceTo(*intersect);

	float dd = a.getGistanceTo(b);

	float h = line.mod();
	float t1 = line.p1.getGistanceTo(*intersect);
	float t2 = line.p2.getGistanceTo(*intersect);

	//return intersect;
	if ((abs(dd - (p1 + p3)) <= eps && abs(dd - (p2 + p4)) <= eps) && abs(h - (t1 + t2)) <= eps) {
		return intersect;
	}
	delete intersect;

	return nullptr;
}
