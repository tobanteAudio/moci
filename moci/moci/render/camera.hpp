#pragma once

#include "moci/app/events/application_event.hpp"
#include "moci/app/events/key_event.hpp"
#include "moci/app/events/mouse_event.hpp"
#include "moci/app/timestep.hpp"

#include "glm/glm.hpp"

namespace moci
{

class OrthographicCamera
{
public:
    OrthographicCamera(float left, float right, float bottom, float top);

    void SetProjection(float left, float right, float bottom, float top);

    const glm::vec3& GetPosition() const { return m_Position; }
    void SetPosition(const glm::vec3& position)
    {
        m_Position = position;
        RecalculateViewMatrix();
    }

    float GetRotation() const { return m_Rotation; }
    void SetRotation(float rotation)
    {
        m_Rotation = rotation;
        RecalculateViewMatrix();
    }

    const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

private:
    void RecalculateViewMatrix();

private:
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ViewProjectionMatrix{};

    glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
    float m_Rotation     = 0.0f;
};

class OrthographicCameraController
{
public:
    OrthographicCameraController(float aspectRatio, bool rotation = false);

    void OnUpdate(Timestep ts);
    void OnEvent(Event& e);

    OrthographicCamera& GetCamera() { return m_Camera; }
    const OrthographicCamera& GetCamera() const { return m_Camera; }

    float GetZoomLevel() const { return m_ZoomLevel; }
    void SetZoomLevel(float level) { m_ZoomLevel = level; }

private:
    bool OnMouseScrolled(MouseScrolledEvent& e);
    bool OnWindowResized(WindowResizeEvent& e);

private:
    float m_AspectRatio;
    float m_ZoomLevel = 1.0f;
    OrthographicCamera m_Camera;

    bool m_Rotation;

    glm::vec3 m_CameraPosition     = {0.0f, 0.0f, 0.0f};
    float m_CameraRotation         = 0.0f;  // In degrees, in the anti-clockwise direction
    float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
};
}  // namespace moci