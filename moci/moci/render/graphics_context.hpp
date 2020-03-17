#pragma once
namespace moci
{

class GraphicsContext
{
public:
    virtual ~GraphicsContext() = default;
    virtual void Init()        = 0;
    virtual void SwapBuffers() = 0;

    static auto Create(void* win) -> GraphicsContext*;
};

}  // namespace moci