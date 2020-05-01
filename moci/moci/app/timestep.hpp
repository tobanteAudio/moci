#pragma once

namespace moci
{

class Timestep
{
public:
    Timestep(float time = 0.0f) : m_Time(time) { }

    operator float() const { return m_Time; }

    [[nodiscard]] auto GetSeconds() const -> float { return m_Time; }
    [[nodiscard]] auto GetMilliseconds() const -> float { return m_Time * 1000.0f; }

private:
    float m_Time;
};

}  // namespace moci