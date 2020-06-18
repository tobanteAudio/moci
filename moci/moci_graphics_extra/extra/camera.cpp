#include "camera.hpp"

#include "moci_core/core/preprocessor.hpp"
#include "moci_events/moci_events.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

namespace moci
{

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
    : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
{
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
{
    m_ProjectionMatrix     = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void OrthographicCamera::RecalculateViewMatrix()
{
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position)
                          * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

    m_ViewMatrix           = glm::inverse(transform);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
    : m_AspectRatio(aspectRatio)
    , m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
    , m_Rotation(rotation)
{
}

void OrthographicCameraController::OnUpdate(Timestep ts)
{
    if (Input::IsKeyPressed(moci::Key::A))
    {
        m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
    }
    else if (Input::IsKeyPressed(moci::Key::D))
    {
        m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
    }

    if (Input::IsKeyPressed(moci::Key::W))
    {
        m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
    }
    else if (Input::IsKeyPressed(moci::Key::S))
    {
        m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
    }

    if (m_Rotation)
    {
        if (Input::IsKeyPressed(moci::Key::Q)) { m_CameraRotation += m_CameraRotationSpeed * ts; }
        if (Input::IsKeyPressed(moci::Key::E)) { m_CameraRotation -= m_CameraRotationSpeed * ts; }

        if (m_CameraRotation > 180.0f) { m_CameraRotation -= 360.0f; }
        else if (m_CameraRotation <= -180.0f)
        {
            m_CameraRotation += 360.0f;
        }

        m_Camera.SetRotation(m_CameraRotation);
    }

    m_Camera.SetPosition(m_CameraPosition);

    m_CameraTranslationSpeed = m_ZoomLevel;
}

void OrthographicCameraController::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(MOCI_EVENT_METHOD(OrthographicCameraController::OnMouseScrolled));
    dispatcher.Dispatch<WindowResizeEvent>(MOCI_EVENT_METHOD(OrthographicCameraController::OnWindowResized));
}

bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
{
    m_ZoomLevel -= e.GetYOffset() * 0.25f;
    m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
    m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    return false;
}

bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
{
    m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
    m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    return false;
}
}  // namespace moci