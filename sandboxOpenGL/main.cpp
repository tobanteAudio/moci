#define MOCI_ENTRYPOINT
#include "moci/moci.hpp"

#include "opengl_layer.hpp"

class Sandbox : public moci::Application
{
public:
    Sandbox() : moci::Application {moci::WindowSpecs {"Sandbox OpenGL", 1920, 1080}}
    {
        MOCI_PROFILE_BEGIN_SESSION("moci-sandbox-opengl", "moci-sandbox-opengl.json");
        GetWindow().SetFullscreen(true);
        GetWindow().SetVSync(true);
        PushLayer(moci::MakeScope<OpenGLLayer>());
    }

    ~Sandbox() override { MOCI_PROFILE_END_SESSION(); }

private:
};

moci::Application* moci::CreateApplication() { return new Sandbox(); }