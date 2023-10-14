#pragma once

namespace moci {

class Timestep
{
public:
    explicit Timestep(float time = 0.0F) : _time(time) {}

    operator float() const { return _time; }  // NOLINT

    [[nodiscard]] auto getSeconds() const -> float { return _time; }

    [[nodiscard]] auto getMilliseconds() const -> float { return _time * 1000.0F; }

private:
    float _time;
};

}  // namespace moci
