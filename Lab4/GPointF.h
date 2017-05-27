#pragma once

struct GPointF {
	GPointF(float x, float y, float z) : x(x), y(y), z(z) {}
	GPointF() : x(), y(), z() {}
	float x, y, z;

	float getGistanceTo(const GPointF &p) const {
		return sqrt(pow(p.x - x, 2) + pow(p.y - y, 2) + pow(p.z - z, 2));
	}

	void normalize() {
		int d = getGistanceTo(GPointF());
		x /= d;
		y /= d;
		z /= d;
	}

	bool operator<(const GPointF &a) const {
		return std::tie(x, y) < std::tie(a.x, a.y);
	}
};