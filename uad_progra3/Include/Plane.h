#pragma once

#include "CVector3.h"

class Plane
{
private:
    // Plane equation
    // Ax + By + Cz + D = 0

public:
    Plane();
    Plane(CVector3 &pt0, CVector3 &pt1, CVector3 &pt2);
    ~Plane() {}

    float A, B, C, D;
    float distanceToPoint(CVector3 &pt);
    void setPoints(CVector3 &pt0, CVector3 &pt1, CVector3 &pt2);
};