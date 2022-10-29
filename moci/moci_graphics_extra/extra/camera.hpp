#pragma once

#include "moci/events/timestep.hpp"
#include "moci/events/types/application_event.hpp"
#include "moci/events/types/key_event.hpp"
#include "moci/events/types/mouse_event.hpp"

#include "glm/glm.hpp"

namespace moci
{

class OrthographicCamera
{
public:
    OrthographicCamera(float left, float right, float bottom, float top);

    void SetProjection(float left, float right, float bottom, float top);

    [[nodiscard]] auto GetPosition() const -> const glm::vec3& { return m_Position; }
    void SetPosition(const glm::vec3& position)
    {
        m_Position = position;
        RecalculateViewMatrix();
    }

    [[nodiscard]] auto GetRotation() const -> float { return m_Rotation; }
    void SetRotation(float rotation)
    {
        m_Rotation = rotation;
        RecalculateViewMatrix();
    }

    [[nodiscard]] auto GetProjectionMatrix() const -> const glm::mat4& { return m_ProjectionMatrix; }
    [[nodiscard]] auto GetViewMatrix() const -> const glm::mat4& { return m_ViewMatrix; }
    [[nodiscard]] auto GetViewProjectionMatrix() const -> const glm::mat4& { return m_ViewProjectionMatrix; }

private:
    void RecalculateViewMatrix();

    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ViewProjectionMatrix {};

    glm::vec3 m_Position = {0.0F, 0.0F, 0.0F};
    float m_Rotation     = 0.0F;
};

class OrthographicCameraController
{
public:
    explicit OrthographicCameraController(float aspectRatio, bool rotation = false);

    void OnUpdate(Timestep ts);
    void OnEvent(Event& e);

    auto GetCamera() -> OrthographicCamera& { return m_Camera; }
    [[nodiscard]] auto GetCamera() const -> const OrthographicCamera& { return m_Camera; }

    [[nodiscard]] auto GetZoomLevel() const -> float { return m_ZoomLevel; }
    void SetZoomLevel(float level) { m_ZoomLevel = level; }

private:
    auto OnMouseScrolled(MouseScrolledEvent& e) -> bool;
    auto OnWindowResized(WindowResizeEvent& e) -> bool;

    float m_AspectRatio;
    float m_ZoomLevel = 1.0F;
    OrthographicCamera m_Camera;

    bool m_Rotation;

    glm::vec3 m_CameraPosition     = {0.0F, 0.0F, 0.0F};
    float m_CameraRotation         = 0.0F;  // In degrees, in the anti-clockwise direction
    float m_CameraTranslationSpeed = 5.0F, m_CameraRotationSpeed = 180.0F;
};
}  // namespace moci