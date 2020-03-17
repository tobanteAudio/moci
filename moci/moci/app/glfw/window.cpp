#include "window.hpp"

#include "moci/core/logging.hpp"

#include "moci/app/events/application_event.hpp"
#include "moci/app/events/key_event.hpp"
#include "moci/app/events/mouse_event.hpp"

namespace moci
{

static uint8_t s_GLFWWindowCount = 0;

static void GLFWErrorCallback(int error, const char* description)
{
    MOCI_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

auto Window::Create(const WindowProps& props) -> Window* { return new GlfwWindow(props); }

GlfwWindow::GlfwWindow(const WindowProps& props) { Init(props); }

GlfwWindow::~GlfwWindow() { Shutdown(); }

void GlfwWindow::Init(const WindowProps& props)
{
    m_Data.Title  = props.Title;
    m_Data.Width  = props.Width;
    m_Data.Height = props.Height;

    MOCI_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

    if (s_GLFWWindowCount == 0)
    {
        MOCI_CORE_INFO("Initializing GLFW");
        int success = glfwInit();
        MOCI_CORE_ASSERT(success, "Could not intialize GLFW!");
        glfwSetErrorCallback(GLFWErrorCallback);
    }

    // msaa
    // glfwWindowHint(GLFW_SAMPLES, 4);

#if defined(MOCI_GRAPHICS_API_GLES)
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    m_Window
        = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), glfwGetPrimaryMonitor(), nullptr);
#else
#if !defined(MOCI_MAC)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif
    m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
#endif

    ++s_GLFWWindowCount;

    m_Context = std::unique_ptr<GraphicsContext>(GraphicsContext::Create(m_Window));
    m_Context->Init();

    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(true);

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.Width       = width;
        data.Height      = height;

        WindowResizeEvent event(width, height);
        data.EventCallback(event);
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowCloseEvent event;
        data.EventCallback(event);
    });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action)
        {
            case GLFW_PRESS:
            {
                KeyPressedEvent event(key, 0);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyReleasedEvent event(key);
                data.EventCallback(event);
                break;
            }
            case GLFW_REPEAT:
            {
                KeyPressedEvent event(key, 1);
                data.EventCallback(event);
                break;
            }
        }
    });

    glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        KeyTypedEvent event(keycode);
        data.EventCallback(event);
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action)
        {
            case GLFW_PRESS:
            {
                MouseButtonPressedEvent event(button);
                data.EventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                MouseButtonReleasedEvent event(button);
                data.EventCallback(event);
                break;
            }
        }
    });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseScrolledEvent event((float)xOffset, (float)yOffset);
        data.EventCallback(event);
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseMovedEvent event((float)xPos, (float)yPos);
        data.EventCallback(event);
    });
}

void GlfwWindow::Shutdown()
{
    glfwDestroyWindow(m_Window);

    if (--s_GLFWWindowCount == 0)
    {
        MOCI_CORE_INFO("Terminating GLFW");
        glfwTerminate();
    }
}

void GlfwWindow::OnUpdate()
{
    glfwPollEvents();
    m_Context->SwapBuffers();
}

void GlfwWindow::SetVSync(bool enabled)
{
    if (enabled)
    {
        glfwSwapInterval(1);
    }
    else
    {
        glfwSwapInterval(0);
    }

    m_Data.VSync = enabled;
}

auto GlfwWindow::IsVSync() const -> bool { return m_Data.VSync; }

}  // namespace moci