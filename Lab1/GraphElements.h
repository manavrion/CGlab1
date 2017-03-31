#pragma once
#include "stdafx.h"

struct GraphElement {

    GraphElement() {};
    
    virtual void paint(Graphics &graphics, PointF center) = 0;

    virtual void paintComplex(Graphics &graphics, PointF center) = 0;
};


struct GraphTargetPoint : public GraphElement {

    GraphTargetPoint(float x, float y, float z) : x(x), y(y), z(z) {};
    GraphTargetPoint(float x, float y, float z, Color color) : x(x), y(y), z(z), color(color) {};
    float x, y, z;
    Color color;

    PointF getProjection(PointF center) {
        center.X += -x;
        center.Y += -z;
        
        center.X += y*0.75;
        center.Y += y*0.75;
        return center;
    }


    void paint(Graphics &graphics, PointF center) {
        PointF pos = getProjection(center);
        graphics.FillEllipse(&SolidBrush(color), RectF(PointF(pos.X - 3, pos.Y - 3), SizeF(6, 6)));
    };

    void paintComplex(Graphics &graphics, PointF center) {
        center.X -= 3;
        center.Y -= 3;
        
        SolidBrush brush(color);

        graphics.FillEllipse(&brush, RectF(PointF(center.X - x, center.Y + y), SizeF(6, 6)));

        graphics.FillEllipse(&brush, RectF(PointF(center.X - x , center.Y - z), SizeF(6, 6)));

        graphics.FillEllipse(&brush, RectF(PointF(center.X + y, center.Y - z), SizeF(6, 6)));
    };

};


struct GraphWrapCube : public GraphElement {

    GraphWrapCube(GraphTargetPoint &target, Color color, int width = 2) : target(target), color(color), width(width) {};

    GraphTargetPoint &target;
    Color color;
    int width;

    void paint(Graphics &graphics, PointF center) {

        GraphTargetPoint target_x(target.x, 0, 0);
        GraphTargetPoint target_y(0, target.y, 0);
        GraphTargetPoint target_z(0, 0, target.z);

        Pen pen(color, width);

        graphics.DrawLine(&pen, target_x.getProjection(center), center);
        graphics.DrawLine(&pen, target_y.getProjection(center), center);
        graphics.DrawLine(&pen, target_z.getProjection(center), center);

        GraphTargetPoint target_xy(target.x, target.y, 0);        
        graphics.DrawLine(&pen, target_x.getProjection(center), target_xy.getProjection(center));
        graphics.DrawLine(&pen, target_y.getProjection(center), target_xy.getProjection(center));

        GraphTargetPoint target_xz(target.x, 0, target.z);
        graphics.DrawLine(&pen, target_x.getProjection(center), target_xz.getProjection(center));
        graphics.DrawLine(&pen, target_z.getProjection(center), target_xz.getProjection(center));

        GraphTargetPoint target_yz(0, target.y, target.z);
        graphics.DrawLine(&pen, target_y.getProjection(center), target_yz.getProjection(center));
        graphics.DrawLine(&pen, target_z.getProjection(center), target_yz.getProjection(center));


        graphics.DrawLine(&pen, target.getProjection(center), target_xy.getProjection(center));
        graphics.DrawLine(&pen, target.getProjection(center), target_xz.getProjection(center));
        graphics.DrawLine(&pen, target.getProjection(center), target_yz.getProjection(center));
        
    };

    void paintComplex(Graphics &graphics, PointF center) {

        PointF pnt1(center.X - target.x, center.Y + target.y);
        PointF pnt2(center.X - target.x, center.Y - target.z);
        PointF pnt3(center.X + target.y, center.Y - target.z);

        Pen pen(color, width);

        graphics.DrawLine(&pen, pnt2, PointF(pnt2.X, center.Y));
        graphics.DrawLine(&pen, pnt2, PointF(center.X, pnt2.Y));

        graphics.DrawLine(&pen, pnt1, PointF(pnt1.X, center.Y));
        graphics.DrawLine(&pen, pnt3, PointF(center.X, pnt3.Y));


        graphics.DrawLine(&pen, pnt1, PointF(center.X, pnt1.Y));
        graphics.DrawLine(&pen, pnt3, PointF(pnt3.X, center.Y));


        graphics.DrawBezier(&pen, PointF(center.X, pnt1.Y), PointF(pnt3.X, pnt1.Y), PointF(pnt3.X, center.Y), PointF(pnt3.X, center.Y));

    };
};



struct GraphLine : public GraphElement {

    GraphLine(GraphTargetPoint a, GraphTargetPoint b, Color color, int width = 2) : a(a), b(b), color(color), width(width) { };

    GraphTargetPoint a;
    GraphTargetPoint b;
    
    Color color;
    int width;

    void paint(Graphics &graphics, PointF center) {
        PointF posA = a.getProjection(center);
        PointF posB = b.getProjection(center);
        graphics.DrawLine(&Pen(color, width), posA, posB);
    };

    void paintComplex(Graphics &graphics, PointF center) {};
};