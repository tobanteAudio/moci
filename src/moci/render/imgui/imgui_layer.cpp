#include "imgui_layer.hpp"

#include <moci/application/application.hpp>
#include <moci/core/benchmark/profile.hpp>
#include <moci/core/logging.hpp>
#include <moci/render/imgui/imgui.hpp>

namespace moci
{

ImGuiLayer::ImGuiLayer() = default;

ImGuiLayer::ImGuiLayer(std::string const& /*unused*/) { }

ImGuiLayer::~ImGuiLayer() = default;

void ImGuiLayer::onAttach()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    auto& app    = Application::get();
    auto* window = static_cast<GLFWwindow*>(app.getWindow().getNativeWindow());

    ImGui_ImplGlfw_InitForOpenGL(window, true);

#if defined(MOCI_MAC)
    ImGui_ImplOpenGL3_Init("#version 100");
#else
    ImGui_ImplOpenGL3_Init("#version 150");
#endif
}

void ImGuiLayer::onDetach()
{

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::begin()
{
    MOCI_PROFILE_FUNCTION();

    {
        MOCI_PROFILE_SCOPE("ImGuiLayer::Begin::NewFrame");

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
    // Docking
    ImGuiWindowFlags windowFlags {};
    static bool optFullscreenPersistant      = true;
    bool optFullscreen                       = optFullscreenPersistant;
    static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (optFullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0F);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0F);
        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
                       | ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if ((dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode) != 0)
    {
        windowFlags |= ImGuiWindowFlags_NoBackground;
    }

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    static auto pOpen = true;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0F, 0.0F));
    ImGui::Begin("DockSpace Demo", &pOpen, windowFlags);
    ImGui::PopStyleVar();

    if (optFullscreen) { ImGui::PopStyleVar(2); }

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if ((io.ConfigFlags & ImGuiConfigFlags_DockingEnable) != 0)
    {
        ImGuiID dockspaceId = ImGui::GetID("dock_space");
        ImGui::DockSpace(dockspaceId, ImVec2(0.0F, 0.0F), dockspaceFlags);
    }
}  // namespace moci

void ImGuiLayer::end()
{
    MOCI_PROFILE_FUNCTION();
    // Docking
    ImGui::End();
    ImGui::Render();

    {
        MOCI_PROFILE_SCOPE("ImGuiLayer::End::RenderDrawData");
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}

void ImGuiLayer::onImGuiRender() { }
}  // namespace moci
