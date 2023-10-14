#pragma once

#include <moci/core/geometry/point.hpp>
#include <moci/core/logging.hpp>

// #include <cmath>

#include <optional>

namespace moci
{
template<typename Type>
/**
 * @todo Write summary for class.
 */
class Line
{
public:
    /**
     * @brief Construct empty line. Start and end at (0,0)
     */
    explicit constexpr Line() noexcept : _start({0, 0}), _end({0, 0}) { }

    /**
     * @brief Construct line from two points.
     */
    constexpr Line(Point<Type> start, Point<Type> end) noexcept : _start(start), _end(end) { }

    /**
     * @brief Returns true if start & end are equal.
     */
    [[nodiscard]] constexpr auto isEmpty() const noexcept -> bool
    {
        return (_start.getX() == _end.getX()) && (_start.getY() == _end.getY());
    }

    /**
     * @brief Returns the start point.
     */
    [[nodiscard]] constexpr auto getStart() const noexcept -> Point<Type> { return _start; }

    /**
     * @brief Returns the end point.
     */
    [[nodiscard]] constexpr auto getEnd() const noexcept -> Point<Type> { return _end; }

    /**
     * @brief Returns the slope. If the line is empty 0 will be returned.
     */
    [[nodiscard]] constexpr auto getSlope() const noexcept -> std::optional<Type>
    {
        if (!isEmpty())
        {
            auto const x = _end.getX() - _start.getX();
            auto const y = _end.getY() - _start.getY();
            if (x == Type(0)) { return std::nullopt; }
            return (y / x);
        }
        return std::nullopt;
    }

    /**
     * @brief Returns the midpoint of the line.
     *
     * @details Midpoint = \f$(x_1+x_2)/2\f$, \f$(y_1+y_2)/2\f$
     */
    [[nodiscard]] constexpr auto getMidPoint() const noexcept -> Point<Type>
    {
        auto const x = (_start.getX() + _end.getX()) / 2.0F;
        auto const y = (_start.getY() + _end.getY()) / 2.0F;
        return Point<Type> {x, y};
    }

    /**
     * @brief Returns the angle of the line in radians.
     */
    [[nodiscard]] auto getAngleRadians() const noexcept -> Type
    {
        auto const angle = std::atan2(_end.getY() - _start.getY(), _end.getX() - _start.getX());
        return static_cast<Type>(angle);
    }

    /**
     * @brief Returns the angle of the line in degrees.
     */
    [[nodiscard]] auto getAngleDegrees() const noexcept -> Type
    {
        return Type(getAngleRadians() * (180.0 / 3.14159265358979323846));
    }

    /**
     * @brief Returns the length of the line.
     *
     * @details The distance between \f$(x_1,y_1)\f$ and \f$(x_2,y_2)\f$ is \f$\sqrt{(x_2-x_1)^2+(y_2-y_1)^2}\f$.
     */
    [[nodiscard]] auto getLength() const noexcept -> Type
    {
        auto const x = std::pow(_end.getX() - _start.getX(), 2.0);
        auto const y = std::pow(_end.getY() - _start.getY(), 2.0);
        return Type(std::sqrt(x + y));
    }

    /**
     * @brief Prints the line in form of: ax + by = c
     */
    void printFormular() const
    {
        auto const a = _end.getY() - _start.getY();
        auto const b = _start.getX() - _end.getX();
        auto const c = a * (_start.getX()) + b * (_start.getY());

        if (b < 0) { fmt::print("The line passing through points P and Q is: {0}x {1}y = {2}\n", a, b, c); }
        else { fmt::print("The line passing through points P and Q is: {0}x + {1}y = {2}\n", a, b, c); }
    }

private:
    Point<Type> _start {};
    Point<Type> _end {};
};

template<typename Type>
auto operator==(Line<Type> const lhs, Line<Type> const rhs) -> bool
{
    auto const start = lhs.getStart() == rhs.getStart();
    auto const end   = lhs.getEnd() == rhs.getEnd();
    return (start && end);
}

template<typename Type>
auto operator!=(Line<Type> const lhs, Line<Type> const rhs) -> bool
{
    return !(lhs == rhs);
}

}  // namespace moci
