#include "../stdafx.h"
#include "../Include/Plane.h"
#include "../Include/CVector3.h"

//
Plane::Plane()
{

}

//
Plane::Plane(CVector3 &pt0, CVector3 &pt1, CVector3 &pt2)
{
    setPoints(pt0, pt1, pt2);
}

// Construimos un plano a partir de 3 puntos que sabemos que estan en el plano
// En un triangulo, los 3 vertices estan en el plano formado por el triangulo, entonces podemos construir el plano de un triangulo a partir de sus vertices
void Plane::setPoints(CVector3 &pt0, CVector3 &pt1, CVector3 &pt2)
{
    CVector3 v = pt1 - pt0;
    CVector3 u = pt2 - pt0;
    CVector3 normal = v.cross(u);
    normal.normalize();

    A = normal.X;
    B = normal.Y;
    C = normal.Z;

    // Ax + By + Cz + D = 0
    // -D = Ax + By + Cz
    // D = -(Ax + By + Cz)
    // Reemplazamos x, y, z por un punto que sabemos que esta en el plano (cualquiera de pt0, pt1, pt2)
    // (A * (pt0.x)) + (B * (pt0.y)) + (C * (pt0.z))
    // Esto es equivalente a Producto Punto
    // Si A, B, C son los componentes de la normal del plano:
    // normal . pt0    
    D = normal.dot(pt0);
    D *= -1.0f;
}

//
float Plane::distanceToPoint(CVector3 &pt)
{
    CVector3 planeNormal;
    planeNormal.X = A;
    planeNormal.Y = B;
    planeNormal.Z = C;

    return(planeNormal.dot(pt) + D);
}