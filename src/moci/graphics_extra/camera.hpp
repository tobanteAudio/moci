#pragma once

#include <moci/events/timestep.hpp>
#include <moci/events/types/application_event.hpp>
#include <moci/events/types/key_event.hpp>
#include <moci/events/types/mouse_event.hpp>

#include "glm/glm.hpp"

namespace moci {

class OrthographicCamera
{
public:
    OrthographicCamera(float left, float right, float bottom, float top);

    void setProjection(float left, float right, float bottom, float top);

    [[nodiscard]] auto getPosition() const -> glm::vec3 const& { return _position; }

    void setPosition(glm::vec3 const& position)
    {
        _position = position;
        recalculateViewMatrix();
    }

    [[nodiscard]] auto getRotation() const -> float { return _rotation; }

    void setRotation(float rotation)
    {
        _rotation = rotation;
        recalculateViewMatrix();
    }

    [[nodiscard]] auto getProjectionMatrix() const -> glm::mat4 const&
    {
        return _projectionMatrix;
    }

    [[nodiscard]] auto getViewMatrix() const -> glm::mat4 const& { return _viewMatrix; }

    [[nodiscard]] auto getViewProjectionMatrix() const -> glm::mat4 const&
    {
        return _viewProjectionMatrix;
    }

private:
    void recalculateViewMatrix();

    glm::mat4 _projectionMatrix;
    glm::mat4 _viewMatrix;
    glm::mat4 _viewProjectionMatrix{};

    glm::vec3 _position = {0.0F, 0.0F, 0.0F};
    float _rotation     = 0.0F;
};

class OrthographicCameraController
{
public:
    explicit OrthographicCameraController(float aspectRatio, bool rotation = false);

    void onUpdate(Timestep ts);
    void onEvent(Event& e);

    auto getCamera() -> OrthographicCamera& { return _camera; }

    [[nodiscard]] auto getCamera() const -> OrthographicCamera const& { return _camera; }

    [[nodiscard]] auto getZoomLevel() const -> float { return _zoomLevel; }

    void setZoomLevel(float level) { _zoomLevel = level; }

private:
    auto onMouseScrolled(MouseScrolledEvent& e) -> bool;
    auto onWindowResized(WindowResizeEvent& e) -> bool;

    float _aspectRatio;
    float _zoomLevel = 1.0F;
    OrthographicCamera _camera;

    bool _rotation;

    glm::vec3 _cameraPosition     = {0.0F, 0.0F, 0.0F};
    float _cameraRotation         = 0.0F;  // In degrees, in the anti-clockwise direction
    float _cameraTranslationSpeed = 5.0F, _cameraRotationSpeed = 180.0F;
};
}  // namespace moci
