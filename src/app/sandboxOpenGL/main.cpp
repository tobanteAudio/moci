#define MOCI_ENTRYPOINT
#include <moci/application/application.hpp>
#include <moci/application/entry_point.hpp>

#include "opengl_layer.hpp"

class Sandbox : public moci::Application
{
public:
    Sandbox() : moci::Application {moci::WindowSpecs {"Sandbox OpenGL", 1920, 1080}}
    {
        MOCI_PROFILE_BEGIN_SESSION("moci-sandbox-opengl", "moci-sandbox-opengl.json");
        // GetWindow().SetFullscreen(true);
        getWindow().setVSync(true);
        pushLayer(std::make_unique<OpenGLLayer>());
    }

    ~Sandbox() override { MOCI_PROFILE_END_SESSION(); }

private:
};

moci::Application* moci::createApplication() { return new Sandbox(); }