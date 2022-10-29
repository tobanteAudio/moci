#pragma once
namespace moci
{

class GraphicsContext
{
public:
    virtual ~GraphicsContext() = default;
    virtual void init()        = 0;
    virtual void swapBuffers() = 0;

    static auto create(void* win) -> GraphicsContext*;
};

}  // namespace moci