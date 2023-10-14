#include "camera.hpp"

#include <moci/core/preprocessor.hpp>
#include <moci/events/input.hpp>
#include <moci/events/types/event.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

namespace moci {

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
    : _projectionMatrix(glm::ortho(left, right, bottom, top, -1.0F, 1.0F))
    , _viewMatrix(1.0F)
{
    _viewProjectionMatrix = _projectionMatrix * _viewMatrix;
}

void OrthographicCamera::setProjection(float left, float right, float bottom, float top)
{
    _projectionMatrix     = glm::ortho(left, right, bottom, top, -1.0F, 1.0F);
    _viewProjectionMatrix = _projectionMatrix * _viewMatrix;
}

void OrthographicCamera::recalculateViewMatrix()
{
    glm::mat4 const transform
        = glm::translate(glm::mat4(1.0F), _position)
        * glm::rotate(glm::mat4(1.0F), glm::radians(_rotation), glm::vec3(0, 0, 1));

    _viewMatrix           = glm::inverse(transform);
    _viewProjectionMatrix = _projectionMatrix * _viewMatrix;
}

OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
    : _aspectRatio(aspectRatio)
    , _camera(
          -_aspectRatio * _zoomLevel,
          _aspectRatio * _zoomLevel,
          -_zoomLevel,
          _zoomLevel
      )
    , _rotation(rotation)
{}

void OrthographicCameraController::onUpdate(Timestep ts)
{
    if (Input::isKeyPressed(moci::Key::A)) {
        _cameraPosition.x
            -= cos(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
        _cameraPosition.y
            -= sin(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
    } else if (Input::isKeyPressed(moci::Key::D)) {
        _cameraPosition.x
            += cos(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
        _cameraPosition.y
            += sin(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
    }

    if (Input::isKeyPressed(moci::Key::W)) {
        _cameraPosition.x
            += -sin(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
        _cameraPosition.y
            += cos(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
    } else if (Input::isKeyPressed(moci::Key::S)) {
        _cameraPosition.x
            -= -sin(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
        _cameraPosition.y
            -= cos(glm::radians(_cameraRotation)) * _cameraTranslationSpeed * ts;
    }

    if (_rotation) {
        if (Input::isKeyPressed(moci::Key::Q)) {
            _cameraRotation += _cameraRotationSpeed * ts;
        }
        if (Input::isKeyPressed(moci::Key::E)) {
            _cameraRotation -= _cameraRotationSpeed * ts;
        }

        if (_cameraRotation > 180.0F) {
            _cameraRotation -= 360.0F;
        } else if (_cameraRotation <= -180.0F) {
            _cameraRotation += 360.0F;
        }

        _camera.setRotation(_cameraRotation);
    }

    _camera.setPosition(_cameraPosition);

    _cameraTranslationSpeed = _zoomLevel;
}

void OrthographicCameraController::onEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<MouseScrolledEvent>(
        MOCI_EVENT_METHOD(OrthographicCameraController::onMouseScrolled)
    );
    dispatcher.dispatch<WindowResizeEvent>(
        MOCI_EVENT_METHOD(OrthographicCameraController::onWindowResized)
    );
}

auto OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e) -> bool
{
    _zoomLevel -= e.getYOffset() * 0.25F;
    _zoomLevel = std::max(_zoomLevel, 0.25F);
    _camera.setProjection(
        -_aspectRatio * _zoomLevel,
        _aspectRatio * _zoomLevel,
        -_zoomLevel,
        _zoomLevel
    );
    return false;
}

auto OrthographicCameraController::onWindowResized(WindowResizeEvent& e) -> bool
{
    _aspectRatio = (float)e.getWidth() / (float)e.getHeight();
    _camera.setProjection(
        -_aspectRatio * _zoomLevel,
        _aspectRatio * _zoomLevel,
        -_zoomLevel,
        _zoomLevel
    );
    return false;
}
}  // namespace moci
