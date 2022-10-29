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

    void setProjection(float left, float right, float bottom, float top);

    [[nodiscard]] auto getPosition() const -> const glm::vec3& { return m_Position; }
    void setPosition(const glm::vec3& position)
    {
        m_Position = position;
        recalculateViewMatrix();
    }

    [[nodiscard]] auto getRotation() const -> float { return m_Rotation; }
    void setRotation(float rotation)
    {
        m_Rotation = rotation;
        recalculateViewMatrix();
    }

    [[nodiscard]] auto getProjectionMatrix() const -> const glm::mat4& { return m_ProjectionMatrix; }
    [[nodiscard]] auto getViewMatrix() const -> const glm::mat4& { return m_ViewMatrix; }
    [[nodiscard]] auto getViewProjectionMatrix() const -> const glm::mat4& { return m_ViewProjectionMatrix; }

private:
    void recalculateViewMatrix();

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

    void onUpdate(Timestep ts);
    void onEvent(Event& e);

    auto getCamera() -> OrthographicCamera& { return m_Camera; }
    [[nodiscard]] auto getCamera() const -> const OrthographicCamera& { return m_Camera; }

    [[nodiscard]] auto getZoomLevel() const -> float { return m_ZoomLevel; }
    void setZoomLevel(float level) { m_ZoomLevel = level; }

private:
    auto onMouseScrolled(MouseScrolledEvent& e) -> bool;
    auto onWindowResized(WindowResizeEvent& e) -> bool;

    float m_AspectRatio;
    float m_ZoomLevel = 1.0F;
    OrthographicCamera m_Camera;

    bool m_Rotation;

    glm::vec3 m_CameraPosition     = {0.0F, 0.0F, 0.0F};
    float m_CameraRotation         = 0.0F;  // In degrees, in the anti-clockwise direction
    float m_CameraTranslationSpeed = 5.0F, m_CameraRotationSpeed = 180.0F;
};
}  // namespace moci