#define MOCI_ENTRYPOINT
#include "moci/moci.hpp"

#include "demo_layer.hpp"

class Sandbox : public moci::Application
{
public:
    Sandbox()
    {
        MOCI_PROFILE_BEGIN_SESSION("moci-sandbox-3d", "moci-sandbox-3d.json");
        PushLayer(moci::MakeScope<DemoLayer>());
    }

    ~Sandbox() override { MOCI_PROFILE_END_SESSION(); }

private:
};

moci::Application* moci::CreateApplication() { return new Sandbox(); }