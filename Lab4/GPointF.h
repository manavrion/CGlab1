#pragma once



struct GPointF {
	GPointF(float x, float y, float z) : x(x), y(y), z(z) {}
	GPointF() : x(), y(), z() {}
	float x, y, z;

	float getGistanceTo(const GPointF &p) const {
		return sqrt(pow(p.x - x, 2) + pow(p.y - y, 2) + pow(p.z - z, 2));
	}

	float hypot() {
		return getGistanceTo(GPointF());
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

	bool operator==(const GPointF &a) const {
		const float eps = 1e-2;
		return getGistanceTo(a) < eps;
	}
};

struct GLine {
	GLine(GPointF p1, GPointF p2) : p1(p1), p2(p2) {};
	
	GPointF p1;
	GPointF p2;

	float mod() const {
		return p1.getGistanceTo(p2);
	}

	float distanceTo(GPointF point) {

		GPointF s(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
		GPointF m1(p1.x, p1.y, p1.z);

		GPointF g(m1.x - point.x, m1.y - point.y, m1.z - point.z);


		GPointF gs(g.y*s.z - s.y*g.z, g.x*s.z - s.x*g.z, g.x*s.y - s.x*g.y);

		return gs.hypot() / s.hypot();
	}
};


struct GPlane {
	GPlane(GPointF a, GPointF b, GPointF c) : a(a), b(b), c(c) {

	};
	GPointF a;
	GPointF b;
	GPointF c;

	GPointF *intersectLineWithTriangle(const GLine &line) {

		GPointF *ret = nullptr;

		const GPointF &m = line.p1;
		const GPointF &n = line.p2;

		float A = (a.y - b.y)*(a.z + b.z) + (b.y - c.y)*(b.z + c.z) + (c.y - a.y)*(c.z + a.z);
		float B = (a.z - b.z)*(a.x + b.x) + (b.z - c.z)*(b.x + c.x) + (c.z - a.z)*(c.x + a.x);
		float C = (a.x - b.x)*(a.y + b.y) + (b.x - c.x)*(b.y + c.y) + (c.x - a.x)*(c.y + a.y);

		float D = -(A*a.x + B*a.y + C*a.z);

		float t = -1;
		float zn = (A*(m.x - n.x) + B*(m.y - n.y) + C*(m.z - n.z));
		if (zn != 0) {
			t = (A*m.x + B*m.y + C*m.z + D) / zn;
		};

		if (t >= 0 && t <= 1) {
			float tx = m.x + (float)(n.x - m.x)*t;
			float ty = m.y + (float)(n.y - m.y)*t;
			float tz = m.z + (float)(n.z - m.z)*t;
			GPointF *intersectPoint = new GPointF(tx, ty, tz);
			ret = intersectPoint;
		}
		return ret;
	}
};



//прямоугольник в пространстве
struct Plate {
	Plate(GPointF a, GPointF b, GPointF c, GPointF d) : a(a), b(b), c(c), d(d) {

	};

	GPointF a;
	GPointF b;
	GPointF c;
	GPointF d;


	GPointF *intersectWithLine(const GLine &line);
};