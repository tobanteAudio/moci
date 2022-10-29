#pragma once

namespace moci
{

class Timestep
{
public:
    explicit Timestep(float time = 0.0F) : m_Time(time) { }

    operator float() const { return m_Time; }  // NOLINT

    [[nodiscard]] auto GetSeconds() const -> float { return m_Time; }
    [[nodiscard]] auto GetMilliseconds() const -> float { return m_Time * 1000.0F; }

private:
    float m_Time;
};

}  // namespace moci
