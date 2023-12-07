#pragma once

#include "CVector3.h"
#include "MathHelper.h"
#include "Frustum.h"
#include "AABB_2D.h"

#define DEFAULT_NEAR_PLANE_DISTANCE 0.1f
#define DEFAULT_FAR_PLANE_DISTANCE 200.0f
#define DEFAULT_FIELD_OF_VIEW 75.0f
#define DEFAULT_CAMERA_SPEED 0.25f

class Camera
{
private:
    CVector3 m_worldUp; // Default 0.0, 1.0, 0.0
    CVector3 m_eyePos, m_lookAt, m_right, m_localUp;
    int m_framebufferWidth, m_framebufferHeight;
    float m_fieldOfView, m_nearPlane, m_farPlane;
    MathHelper::Matrix4 m_viewMatrix;
    MathHelper::Matrix4 m_projectionMatrix;
    ViewFrustum m_viewFrustum;
    bool m_cacheValid;

    void recalculateViewMatrix();
    void recalculateProjectionMatrix();
    void recalculateViewFrustum();

public:
    Camera(int framebufferWidth, int framebufferHeight);
    Camera(int framebufferWidth, int framebufferHeight, float fov, float nearP, float farP);
    Camera(int framebufferWidth, int framebufferHeight, float fov, float nearP, float farP, CVector3 cameraPos, CVector3 cameraLookAt, CVector3 cameraUp);
    ~Camera() {}

    // 
    void updateFramebufferSize(int width, int height);
    void update(double delta_time);

    // Movimiento
    void moveForward(float deltaMove, int mods); //
    void moveBackward(float deltaMove, int mods);
    void moveUp(float deltaMove, int mods);
    void moveDown(float deltaMove, int mods);
    void strafe(float deltaMove, int mods);

    // Test de visibilidad
    bool isAABBVisible(AABB_2D &aabb); //

    //
    const CVector3& getEyePos() const { return m_eyePos; }
    const CVector3& getLookAt() const { return m_lookAt; }
    const CVector3& getUpVec() const { return m_localUp; }
    const MathHelper::Matrix4* getViewMatrix();
    const MathHelper::Matrix4* getProjectionMatrix() const { return &m_projectionMatrix; }
};


