#include "window.hpp"

#include "moci/core/logging.hpp"
#include "moci/events/types/application_event.hpp"
#include "moci/events/types/key_event.hpp"
#include "moci/events/types/mouse_event.hpp"

namespace moci
{

static uint8_t sGlfwWindowCount = 0;

static void glfwErrorCallback(int error, const char* description)
{
    MOCI_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
}

auto Window::create(WindowSpecs props) -> Window* { return new GlfwWindow(std::move(props)); }

GlfwWindow::GlfwWindow(WindowSpecs props) { init(std::move(props)); }

GlfwWindow::~GlfwWindow() { shutdown(); }

void GlfwWindow::init(WindowSpecs props)
{
    m_Data.Title  = props.Title;
    m_Data.Width  = props.Width;
    m_Data.Height = props.Height;

    if (sGlfwWindowCount == 0)
    {
        MOCI_CORE_INFO("Initializing GLFW");
        int success = glfwInit();
        MOCI_CORE_ASSERT(success, "Could not intialize GLFW!");
        glfwSetErrorCallback(glfwErrorCallback);
    }

#if defined(MOCI_API_OPENGL_ES)
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#else
#if not defined(MOCI_MAC)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif
#endif

    auto const width  = static_cast<int>(m_Data.Width);
    auto const height = static_cast<int>(m_Data.Height);
    m_Window          = glfwCreateWindow(width, height, m_Data.Title.c_str(), nullptr, nullptr);
    ++sGlfwWindowCount;
    MOCI_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

    m_Context = Scope<GraphicsContext>(GraphicsContext::create(m_Window));
    m_Context->init();

    glfwSetWindowUserPointer(m_Window, &m_Data);
    setVSync(true);

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(m_Window,
                              [](GLFWwindow* window, int width, int height)
                              {
                                  WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                                  data.Width       = width;
                                  data.Height      = height;

                                  WindowResizeEvent event(width, height);
                                  data.EventCallback(event);
                              });

    glfwSetWindowCloseCallback(m_Window,
                               [](GLFWwindow* window)
                               {
                                   WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                                   WindowCloseEvent event;
                                   data.EventCallback(event);
                               });

    glfwSetKeyCallback(m_Window,
                       [](GLFWwindow* window, int glfwKey, int scancode, int action, int mods)
                       {
                           ignoreUnused(scancode);
                           ignoreUnused(mods);

                           WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                           auto const key   = static_cast<Key>(glfwKey);
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

    glfwSetCharCallback(m_Window,
                        [](GLFWwindow* window, unsigned int glfwKeyCode)
                        {
                            auto event = KeyTypedEvent {static_cast<Key>(glfwKeyCode)};
                            auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                            data.EventCallback(event);
                        });

    glfwSetMouseButtonCallback(m_Window,
                               [](GLFWwindow* window, int button, int action, int mods)
                               {
                                   ignoreUnused(mods);
                                   auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

                                   switch (action)
                                   {
                                       case GLFW_PRESS:
                                       {
                                           MouseButtonPressedEvent event(static_cast<MouseCode>(button));
                                           data.EventCallback(event);
                                           break;
                                       }
                                       case GLFW_RELEASE:
                                       {
                                           MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
                                           data.EventCallback(event);
                                           break;
                                       }
                                   }
                               });

    glfwSetScrollCallback(m_Window,
                          [](GLFWwindow* window, double xOffset, double yOffset)
                          {
                              WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                              MouseScrolledEvent event((float)xOffset, (float)yOffset);
                              data.EventCallback(event);
                          });

    glfwSetCursorPosCallback(m_Window,
                             [](GLFWwindow* window, double xPos, double yPos)
                             {
                                 WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                                 MouseMovedEvent event((float)xPos, (float)yPos);
                                 data.EventCallback(event);
                             });
}

void GlfwWindow::shutdown()
{
    glfwDestroyWindow(m_Window);

    if (--sGlfwWindowCount == 0)
    {
        MOCI_CORE_INFO("Terminating GLFW");
        glfwTerminate();
    }
}

void GlfwWindow::onUpdate()
{
    frameCounter_++;
    glfwPollEvents();
    m_Context->swapBuffers();
}

void GlfwWindow::setVSync(bool enabled)
{
    if (enabled) { glfwSwapInterval(1); }
    else { glfwSwapInterval(0); }

    m_Data.VSync = enabled;
}

auto GlfwWindow::isVSync() const -> bool { return m_Data.VSync; }

void GlfwWindow::setFullscreen(bool enabled)
{
    if (isFullscreen() == enabled) { return; }
    MOCI_CORE_INFO("Setting window fullscreen to {}", enabled);

    if (enabled)
    {
        // backup window position and window size
        auto x = 0;
        auto y = 0;
        glfwGetWindowPos(m_Window, (m_Data.Position).data(), &m_Data.Position[1]);
        glfwGetWindowSize(m_Window, &x, &y);
        m_Data.Width  = static_cast<unsigned>(x);
        m_Data.Height = static_cast<unsigned>(y);

        // get resolution of monitor
        auto* monitor    = glfwGetPrimaryMonitor();
        auto const* mode = glfwGetVideoMode(monitor);

        // switch to full screen
        glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, 0);
    }
    else
    {
        // restore last window size and position
        glfwSetWindowMonitor(m_Window, nullptr, m_Data.Position[0], m_Data.Position[1], m_Data.Width, m_Data.Height, 0);
    }

    m_Data.Fullscreen = enabled;
}

auto GlfwWindow::isFullscreen() const -> bool { return m_Data.Fullscreen; }

}  // namespace moci
