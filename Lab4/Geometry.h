#pragma once

namespace Geomenty {

	struct GPointF {
		GPointF(float x, float y, float z) : x(x), y(y), z(z) {}
		GPointF() : x(), y(), z() {}
		float x, y, z;

		float getDistanceTo(const GPointF &p) const {
			return sqrt(pow(p.x - x, 2) + pow(p.y - y, 2) + pow(p.z - z, 2));
		}

		float hypot() const {
			return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
		}

		void normalize() {
			int d = getDistanceTo(GPointF());
			x /= d;
			y /= d;
			z /= d;
		}

		bool operator<(const GPointF &a) const {
			return std::tie(x, y) < std::tie(a.x, a.y);
		}

		bool operator==(const GPointF &a) const {
			const float eps = 1e-2;
			return getDistanceTo(a) < eps;
		}
	};

	struct GLine {
		GLine(const GPointF &p1, const GPointF &p2) : p1(p1), p2(p2) {};

		GPointF p1;
		GPointF p2;

		float getLength() const {
			return p1.getDistanceTo(p2);
		}

		float getDistanceTo(const GPointF &point) const {

			GPointF s(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
			GPointF m1(p1.x, p1.y, p1.z);

			GPointF g(m1.x - point.x, m1.y - point.y, m1.z - point.z);

			GPointF gs(g.y*s.z - s.y*g.z, g.x*s.z - s.x*g.z, g.x*s.y - s.x*g.y);

			return gs.hypot() / s.hypot();
		}
	};


	struct GPlane {
		GPlane(const GPointF &a, const GPointF &b, const GPointF &c) : a(a), b(b), c(c) {}

		GPointF a;
		GPointF b;
		GPointF c;

		GPointF *intersectLineWithTriangle(const GLine &line) const {

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


	struct GPlate {
		GPlate(const GPointF &a, const GPointF &b, const GPointF &c, const GPointF &d) : a(a), b(b), c(c), d(d) {}

		GPointF a;
		GPointF b;
		GPointF c;
		GPointF d;

		GPointF *intersectWithLine(const GLine &line) const {

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

			float p1 = l1.getDistanceTo(*intersect);
			float p2 = l2.getDistanceTo(*intersect);
			float p3 = l3.getDistanceTo(*intersect);
			float p4 = l4.getDistanceTo(*intersect);

			float pd = a.getDistanceTo(b);

			float td = line.getLength();
			float t1 = line.p1.getDistanceTo(*intersect);
			float t2 = line.p2.getDistanceTo(*intersect);

			const float eps = 1e-2;
			if ((abs(pd - (p1 + p3)) <= eps && abs(pd - (p2 + p4)) <= eps) && abs(td - (t1 + t2)) <= eps) {
				return intersect;
			}

			delete intersect;
			return nullptr;
		}
	};

}