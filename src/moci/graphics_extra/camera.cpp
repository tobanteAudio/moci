#include "camera.hpp"

#include "moci/core/preprocessor.hpp"
#include "moci/events/input.hpp"
#include "moci/events/types/event.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

namespace moci
{

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
    : _m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0F, 1.0F)), _m_ViewMatrix(1.0F)
{
    _m_ViewProjectionMatrix = _m_ProjectionMatrix * _m_ViewMatrix;
}

void OrthographicCamera::setProjection(float left, float right, float bottom, float top)
{
    _m_ProjectionMatrix     = glm::ortho(left, right, bottom, top, -1.0F, 1.0F);
    _m_ViewProjectionMatrix = _m_ProjectionMatrix * _m_ViewMatrix;
}

void OrthographicCamera::recalculateViewMatrix()
{
    glm::mat4 transform = glm::translate(glm::mat4(1.0F), _m_Position)
                          * glm::rotate(glm::mat4(1.0F), glm::radians(_m_Rotation), glm::vec3(0, 0, 1));

    _m_ViewMatrix           = glm::inverse(transform);
    _m_ViewProjectionMatrix = _m_ProjectionMatrix * _m_ViewMatrix;
}

OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
    : _m_AspectRatio(aspectRatio)
    , _m_Camera(-_m_AspectRatio * _m_ZoomLevel, _m_AspectRatio * _m_ZoomLevel, -_m_ZoomLevel, _m_ZoomLevel)
    , _m_Rotation(rotation)
{
}

void OrthographicCameraController::onUpdate(Timestep ts)
{
    if (Input::isKeyPressed(moci::Key::A))
    {
        _m_CameraPosition.x -= cos(glm::radians(_m_CameraRotation)) * _m_CameraTranslationSpeed * ts;
        _m_CameraPosition.y -= sin(glm::radians(_m_CameraRotation)) * _m_CameraTranslationSpeed * ts;
    }
    else if (Input::isKeyPressed(moci::Key::D))
    {
        _m_CameraPosition.x += cos(glm::radians(_m_CameraRotation)) * _m_CameraTranslationSpeed * ts;
        _m_CameraPosition.y += sin(glm::radians(_m_CameraRotation)) * _m_CameraTranslationSpeed * ts;
    }

    if (Input::isKeyPressed(moci::Key::W))
    {
        _m_CameraPosition.x += -sin(glm::radians(_m_CameraRotation)) * _m_CameraTranslationSpeed * ts;
        _m_CameraPosition.y += cos(glm::radians(_m_CameraRotation)) * _m_CameraTranslationSpeed * ts;
    }
    else if (Input::isKeyPressed(moci::Key::S))
    {
        _m_CameraPosition.x -= -sin(glm::radians(_m_CameraRotation)) * _m_CameraTranslationSpeed * ts;
        _m_CameraPosition.y -= cos(glm::radians(_m_CameraRotation)) * _m_CameraTranslationSpeed * ts;
    }

    if (_m_Rotation)
    {
        if (Input::isKeyPressed(moci::Key::Q)) { _m_CameraRotation += _m_CameraRotationSpeed * ts; }
        if (Input::isKeyPressed(moci::Key::E)) { _m_CameraRotation -= _m_CameraRotationSpeed * ts; }

        if (_m_CameraRotation > 180.0F) { _m_CameraRotation -= 360.0F; }
        else if (_m_CameraRotation <= -180.0F) { _m_CameraRotation += 360.0F; }

        _m_Camera.setRotation(_m_CameraRotation);
    }

    _m_Camera.setPosition(_m_CameraPosition);

    _m_CameraTranslationSpeed = _m_ZoomLevel;
}

void OrthographicCameraController::onEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<MouseScrolledEvent>(MOCI_EVENT_METHOD(OrthographicCameraController::onMouseScrolled));
    dispatcher.dispatch<WindowResizeEvent>(MOCI_EVENT_METHOD(OrthographicCameraController::onWindowResized));
}

auto OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e) -> bool
{
    _m_ZoomLevel -= e.getYOffset() * 0.25F;
    _m_ZoomLevel = std::max(_m_ZoomLevel, 0.25F);
    _m_Camera.setProjection(-_m_AspectRatio * _m_ZoomLevel, _m_AspectRatio * _m_ZoomLevel, -_m_ZoomLevel, _m_ZoomLevel);
    return false;
}

auto OrthographicCameraController::onWindowResized(WindowResizeEvent& e) -> bool
{
    _m_AspectRatio = (float)e.getWidth() / (float)e.getHeight();
    _m_Camera.setProjection(-_m_AspectRatio * _m_ZoomLevel, _m_AspectRatio * _m_ZoomLevel, -_m_ZoomLevel, _m_ZoomLevel);
    return false;
}
}  // namespace moci