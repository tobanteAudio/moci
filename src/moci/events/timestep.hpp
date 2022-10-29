#pragma once

namespace moci
{

class Timestep
{
public:
    explicit Timestep(float time = 0.0F) : _m_Time(time) { }

    operator float() const { return _m_Time; }  // NOLINT

    [[nodiscard]] auto getSeconds() const -> float { return _m_Time; }
    [[nodiscard]] auto getMilliseconds() const -> float { return _m_Time * 1000.0F; }

private:
    float _m_Time;
};

}  // namespace moci
