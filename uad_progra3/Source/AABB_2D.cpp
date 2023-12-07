#include "../stdafx.h"
#include "../Include/AABB_2D.h"


AABB_2D::AABB_2D()
{
    m_points[0] = CVector3::ZeroVector();
    m_points[1] = CVector3::ZeroVector();
    m_points[2] = CVector3::ZeroVector();
    m_points[3] = CVector3::ZeroVector();
}

void AABB_2D::setPoints(CVector3* points)
{
    m_points[0] = *(points);     // Top Left
    m_points[1] = *(points + 1); // Top Right
    m_points[2] = *(points + 2); // Bottom Left
    m_points[3] = *(points + 3); // Bottom Right
}
