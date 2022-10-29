#include "camera.hpp"

#include "moci/core/preprocessor.hpp"
#include "moci/events/input.hpp"
#include "moci/events/types/event.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

namespace moci
{

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
    : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0F, 1.0F)), m_ViewMatrix(1.0F)
{
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void OrthographicCamera::setProjection(float left, float right, float bottom, float top)
{
    m_ProjectionMatrix     = glm::ortho(left, right, bottom, top, -1.0F, 1.0F);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void OrthographicCamera::recalculateViewMatrix()
{
    glm::mat4 transform = glm::translate(glm::mat4(1.0F), m_Position)
                          * glm::rotate(glm::mat4(1.0F), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

    m_ViewMatrix           = glm::inverse(transform);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
    : m_AspectRatio(aspectRatio)
    , m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
    , m_Rotation(rotation)
{
}

void OrthographicCameraController::onUpdate(Timestep ts)
{
    if (Input::isKeyPressed(moci::Key::A))
    {
        m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
    }
    else if (Input::isKeyPressed(moci::Key::D))
    {
        m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
    }

    if (Input::isKeyPressed(moci::Key::W))
    {
        m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
    }
    else if (Input::isKeyPressed(moci::Key::S))
    {
        m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
    }

    if (m_Rotation)
    {
        if (Input::isKeyPressed(moci::Key::Q)) { m_CameraRotation += m_CameraRotationSpeed * ts; }
        if (Input::isKeyPressed(moci::Key::E)) { m_CameraRotation -= m_CameraRotationSpeed * ts; }

        if (m_CameraRotation > 180.0F) { m_CameraRotation -= 360.0F; }
        else if (m_CameraRotation <= -180.0F) { m_CameraRotation += 360.0F; }

        m_Camera.setRotation(m_CameraRotation);
    }

    m_Camera.setPosition(m_CameraPosition);

    m_CameraTranslationSpeed = m_ZoomLevel;
}

void OrthographicCameraController::onEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<MouseScrolledEvent>(MOCI_EVENT_METHOD(OrthographicCameraController::onMouseScrolled));
    dispatcher.dispatch<WindowResizeEvent>(MOCI_EVENT_METHOD(OrthographicCameraController::onWindowResized));
}

auto OrthographicCameraController::onMouseScrolled(MouseScrolledEvent& e) -> bool
{
    m_ZoomLevel -= e.getYOffset() * 0.25F;
    m_ZoomLevel = std::max(m_ZoomLevel, 0.25F);
    m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    return false;
}

auto OrthographicCameraController::onWindowResized(WindowResizeEvent& e) -> bool
{
    m_AspectRatio = (float)e.getWidth() / (float)e.getHeight();
    m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    return false;
}
}  // namespace moci