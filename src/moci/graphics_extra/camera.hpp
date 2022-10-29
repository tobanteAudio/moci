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

    [[nodiscard]] auto getPosition() const -> const glm::vec3& { return _m_Position; }
    void setPosition(const glm::vec3& position)
    {
        _m_Position = position;
        recalculateViewMatrix();
    }

    [[nodiscard]] auto getRotation() const -> float { return _m_Rotation; }
    void setRotation(float rotation)
    {
        _m_Rotation = rotation;
        recalculateViewMatrix();
    }

    [[nodiscard]] auto getProjectionMatrix() const -> const glm::mat4& { return _m_ProjectionMatrix; }
    [[nodiscard]] auto getViewMatrix() const -> const glm::mat4& { return _m_ViewMatrix; }
    [[nodiscard]] auto getViewProjectionMatrix() const -> const glm::mat4& { return _m_ViewProjectionMatrix; }

private:
    void recalculateViewMatrix();

    glm::mat4 _m_ProjectionMatrix;
    glm::mat4 _m_ViewMatrix;
    glm::mat4 _m_ViewProjectionMatrix {};

    glm::vec3 _m_Position = {0.0F, 0.0F, 0.0F};
    float _m_Rotation     = 0.0F;
};

class OrthographicCameraController
{
public:
    explicit OrthographicCameraController(float aspectRatio, bool rotation = false);

    void onUpdate(Timestep ts);
    void onEvent(Event& e);

    auto getCamera() -> OrthographicCamera& { return _m_Camera; }
    [[nodiscard]] auto getCamera() const -> const OrthographicCamera& { return _m_Camera; }

    [[nodiscard]] auto getZoomLevel() const -> float { return _m_ZoomLevel; }
    void setZoomLevel(float level) { _m_ZoomLevel = level; }

private:
    auto onMouseScrolled(MouseScrolledEvent& e) -> bool;
    auto onWindowResized(WindowResizeEvent& e) -> bool;

    float _m_AspectRatio;
    float _m_ZoomLevel = 1.0F;
    OrthographicCamera _m_Camera;

    bool _m_Rotation;

    glm::vec3 _m_CameraPosition     = {0.0F, 0.0F, 0.0F};
    float _m_CameraRotation         = 0.0F;  // In degrees, in the anti-clockwise direction
    float _m_CameraTranslationSpeed = 5.0F, _m_CameraRotationSpeed = 180.0F;
};
}  // namespace moci