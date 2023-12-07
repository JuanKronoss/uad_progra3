#include "../stdafx.h"
#include "../Include/CCamera.h"
#include "../Include/CApp.h"

Camera::Camera(int framebufferWidth, int framebufferHeight) :
    Camera(framebufferWidth, framebufferHeight, DEFAULT_FIELD_OF_VIEW, DEFAULT_NEAR_PLANE_DISTANCE, DEFAULT_FAR_PLANE_DISTANCE,
        CVector3(0.0f, 5.0f, 5.0f), CVector3(0.0f, 0.0f, 0.0f), CVector3(0.0f, 1.0f, 0.0f))
{    
}

Camera::Camera(int framebufferWidth, int framebufferHeight, float fov, float nearP, float farP)
    : Camera(framebufferWidth, framebufferHeight, fov, nearP, farP, CVector3(0.0f, 5.0f, 5.0f), CVector3(0.0f, 0.0f, 0.0f), CVector3(0.0f, 1.0f, 0.0f))
{

}

Camera::Camera(int framebufferWidth, int framebufferHeight, float fov, float nearP, float farP, CVector3 cameraPos, CVector3 cameraLookAt, CVector3 cameraUp) :
    m_framebufferWidth(framebufferWidth),
    m_framebufferHeight(framebufferHeight),
    m_fieldOfView(fov),
    m_nearPlane(nearP),
    m_farPlane(farP),
    m_eyePos(cameraPos),
    m_lookAt(cameraLookAt),
    m_worldUp(cameraUp),
    m_cacheValid(false)
{
    recalculateProjectionMatrix();
    recalculateViewMatrix();
    recalculateViewFrustum();
}

void Camera::updateFramebufferSize(int width, int height)
{
    m_framebufferWidth = width;
    m_framebufferHeight = height;
    recalculateProjectionMatrix();
}

void Camera::update(double delta_time)
{
}

void Camera::recalculateProjectionMatrix()
{
    if (m_framebufferHeight > 0)
    {
        m_projectionMatrix = MathHelper::PerspectiveProjectionMatrix(m_fieldOfView, (m_framebufferWidth / m_framebufferHeight), m_nearPlane, m_farPlane);
    }
}

void Camera::recalculateViewMatrix()
{
    CVector3 forward = m_lookAt - m_eyePos;
    forward.normalize();

    //
    m_right = m_worldUp.cross(forward);
    m_right.normalize();
    // m_right.X *= -1.0f;

    m_localUp = forward.cross(m_right);

    m_right.X = abs(m_right.X);
    m_right.Y = abs(m_right.Y);
    m_right.Z = abs(m_right.Z);

    m_viewMatrix = MathHelper::ViewMatrix(m_eyePos, m_lookAt, m_localUp);

    m_cacheValid = true;
}

void Camera::recalculateViewFrustum()
{
    if (m_framebufferHeight > 0)
    {
        m_viewFrustum.update(m_eyePos, m_lookAt, m_localUp, m_right, m_nearPlane, m_farPlane, m_fieldOfView, (m_framebufferWidth / m_framebufferHeight));
    }
}

void Camera::moveForward(float deltaMove, int mods)
{
    if (fabs(deltaMove) > 0.001f)
    {
        // Calcular el forward vector
        CVector3 forward = m_lookAt - m_eyePos;
        forward.normalize();

        if (mods & KEY_MOD_SHIFT)
        {
            deltaMove *= 2.5f;
        }

        forward *= deltaMove;

        m_eyePos += forward;
        m_lookAt += forward;

        // Recalcular la view-matrix cuando se llame getViewMatrix()
        //getViewMatrix();
        m_cacheValid = false;
    }
}

void Camera::moveBackward(float deltaMove, int mods)
{
    if (fabs(deltaMove) > 0.001f)
    {
        // Calcular el forward vector
        CVector3 forward = m_lookAt - m_eyePos;
        forward.normalize();

        if (mods & KEY_MOD_SHIFT)
        {
            deltaMove *= 2.5f;
        }

        //Invertir la dirección del vector para ir hacia atrás
        deltaMove *= -1;
        forward *= deltaMove;

        m_eyePos += forward;
        m_lookAt += forward;

        // Recalcular la view-matrix cuando se llame getViewMatrix()
        //getViewMatrix();
        m_cacheValid = false;
    }
}

void Camera::moveUp(float deltaMove, int mods)
{
    if (fabs(deltaMove) > 0.001f)
    {
        CVector3 upVector = m_worldUp;
        upVector.normalize();

        if (mods & KEY_MOD_SHIFT)
        {
            deltaMove *= 2.5f;
        }

        upVector *= deltaMove;

        m_eyePos += upVector;
        m_lookAt += upVector;

        // Recalcular la view-matrix cuando se llame getViewMatrix()
        //getViewMatrix();
        m_cacheValid = false;
    }

    //if (fabs(deltaMove) > 0.001f)
    //{
    //    // Calcular el forward vector
    //    CVector3 forward = m_lookAt - m_eyePos;
    //    forward.normalize();

    //    //Obtener el vector perpendicular al Up para moverse hacia los lados
    //    CVector3 upVector = m_eyePos.cross(m_lookAt);
    //    upVector.normalize();

    //    if (mods & KEY_MOD_SHIFT)
    //    {
    //        deltaMove *= 2.5f;
    //    }

    //    upVector *= deltaMove;

    //    m_eyePos += upVector;
    //    m_lookAt += upVector;

    //    // Recalcular la view-matrix cuando se llame getViewMatrix()
    //    m_cacheValid = false;
    //}
}

void Camera::moveDown(float deltaMove, int mods)
{
    if (fabs(deltaMove) > 0.001f)
    {
        CVector3 downVector = m_worldUp;
        downVector.normalize();

        if (mods & KEY_MOD_SHIFT)
        {
            deltaMove *= 2.5f;
        }

        downVector *= deltaMove;
        downVector *= -1;

        m_eyePos += downVector;
        m_lookAt += downVector;

        // Recalcular la view-matrix cuando se llame getViewMatrix()
        //getViewMatrix();
        m_cacheValid = false;
    }
}

void Camera::strafe(float deltaMove, int mods)
{
    if (fabs(deltaMove) > 0.001f)
    {
        // Calcular el forward vector
        CVector3 forward = m_lookAt - m_eyePos;
        forward.normalize();

        //Obtener el vector perpendicular al Up para moverse hacia los lados
        CVector3 rightVector = m_worldUp.cross(forward);
        rightVector.normalize();

        if (mods & KEY_MOD_SHIFT)
        {
            deltaMove *= 2.5f;
        }

        rightVector *= deltaMove;

        m_eyePos += rightVector;
        m_lookAt += rightVector;

        // Recalcular la view-matrix cuando se llame getViewMatrix()
        //getViewMatrix();
        m_cacheValid = false;
    }
}

bool Camera::isAABBVisible(AABB_2D &aabb)
{
    return (
        m_viewFrustum.isAABBVisible(aabb) == ViewFrustum::FRUSTUM_VISIBILITY_TEST_RESULT::INSIDE ||
        m_viewFrustum.isAABBVisible(aabb) == ViewFrustum::FRUSTUM_VISIBILITY_TEST_RESULT::INTERSECT
        );
}

const MathHelper::Matrix4 * Camera::getViewMatrix()
{
    if (!m_cacheValid)
    {
        recalculateViewMatrix();
        recalculateViewFrustum();
    }

    return &m_viewMatrix;
}