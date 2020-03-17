#include "imgui_layer.hpp"

#include "moci/app/application.hpp"
#include "moci/core/logging.hpp"

namespace moci
{

ImGuiLayer::ImGuiLayer() = default;

ImGuiLayer::ImGuiLayer(std::string const& /*unused*/) {}

ImGuiLayer::~ImGuiLayer() = default;

void ImGuiLayer::OnAttach()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    auto& app    = Application::Get();
    auto* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

    ImGui_ImplGlfw_InitForOpenGL(window, true);

#if !defined(MOCI_MAC)
    ImGui_ImplOpenGL3_Init("#version 100");
#else
    ImGui_ImplOpenGL2_Init();
#endif
}

void ImGuiLayer::OnDetach()
{
#if !defined(MOCI_MAC)
    ImGui_ImplOpenGL3_Shutdown();
#else
    ImGui_ImplOpenGL2_Shutdown();
#endif
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::Begin()
{
#if !defined(MOCI_MAC)
    ImGui_ImplOpenGL3_NewFrame();
#else
    ImGui_ImplOpenGL2_NewFrame();
#endif
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::End()
{
    ImGui::Render();
#if !defined(MOCI_MAC)
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#else
    GLint last_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
    glUseProgram(0);
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
    glUseProgram(last_program);
#endif
}

void ImGuiLayer::OnImGuiRender()
{
    auto const fps       = ImGui::GetIO().Framerate;
    auto const fpsString = fmt::format("{0:0.1f} FPS", fps);

    // Main menu
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Debug"))
        {
            ImGui::Checkbox("Imgui Demo", &m_show_imgui_demo);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu(fpsString.c_str()))
        {
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // ImGui Demo
    if (m_show_imgui_demo)
    {
        ImGui::ShowDemoWindow(&m_show_imgui_demo);
    }
}
}  // namespace moci