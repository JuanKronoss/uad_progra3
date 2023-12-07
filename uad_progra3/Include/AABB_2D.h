#pragma once

#include "CVector3.h"

// Axis-Aligned Bounding Box
class AABB_2D
{
public:
    CVector3 m_points[4];
    AABB_2D();
    ~AABB_2D() {}
    void setPoints(CVector3* points);
};