#pragma once

#include "CVector3.h"
#include "MathHelper.h"
#include "AABB_2D.h"
#include "Plane.h"

class ViewFrustum
{
private:

    enum FRUSTUM_PLANE_ID
    {
        TOP_PLANE = 0,
        BOTTOM_PLANE,
        LEFT_PLANE,
        RIGHT_PLANE,
        NEAR_PLANE,
        FAR_PLANE
    };


public:
    Plane m_planes[6];

    enum FRUSTUM_VISIBILITY_TEST_RESULT
    {
        OUTSIDE = 0,
        INSIDE,
        INTERSECT
    };

    ViewFrustum();
    ~ViewFrustum();

    FRUSTUM_VISIBILITY_TEST_RESULT isAABBVisible(AABB_2D &aabb); // Llama a isPointVisible() para cada una de las esquinas del AABB
    FRUSTUM_VISIBILITY_TEST_RESULT isPointVisible(CVector3 &point);

    void update(CVector3 eyePos, CVector3 lookAtPos, CVector3 upVector, CVector3 rightVec, float nearDistance, float farDistance, float fov, float aspectRatio);
};
