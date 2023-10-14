#define MOCI_ENTRYPOINT
#include <moci/application/application.hpp>
#include <moci/application/entry_point.hpp>

#include "demo_layer.hpp"

class Sandbox : public moci::Application
{
public:
    Sandbox()
        : moci::Application{
            moci::WindowSpecs{"Sandbox 3D", 1920, 1080}
    }
    {
        MOCI_PROFILE_BEGIN_SESSION("moci-sandbox-3d", "moci-sandbox-3d.json");
        getWindow().setFullscreen(true);
        getWindow().setVSync(false);
        pushLayer(std::make_unique<DemoLayer>());
    }

    ~Sandbox() override { MOCI_PROFILE_END_SESSION(); }

private:
};

auto moci::createApplication() -> moci::Application* { return new Sandbox(); }
