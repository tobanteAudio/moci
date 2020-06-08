#pragma once

#include "moci_core/core/logging.hpp"
#include "moci_math/geometry/point.hpp"

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
    explicit constexpr Line() noexcept : start_({0, 0}), end_({0, 0}) { }

    /**
     * @brief Construct line from two points.
     */
    constexpr Line(Point<Type> start, Point<Type> end) noexcept : start_(start), end_(end) { }

    /**
     * @brief Returns true if start & end are equal.
     */
    [[nodiscard]] constexpr bool IsEmpty() const noexcept
    {
        return (start_.GetX() == end_.GetX()) && (start_.GetY() == end_.GetY());
    }

    /**
     * @brief Returns the start point.
     */
    [[nodiscard]] constexpr Point<Type> GetStart() const noexcept { return start_; }

    /**
     * @brief Returns the end point.
     */
    [[nodiscard]] constexpr Point<Type> GetEnd() const noexcept { return end_; }

    /**
     * @brief Returns the slope. If the line is empty 0 will be returned.
     */
    [[nodiscard]] constexpr std::optional<Type> GetSlope() const noexcept
    {
        if (!IsEmpty())
        {
            auto const x = end_.GetX() - start_.GetX();
            auto const y = end_.GetY() - start_.GetY();
            if (x == Type(0))
            {
                return std::nullopt;
            }
            return (y / x);
        }
        return std::nullopt;
    }

    /**
     * @brief Returns the midpoint of the line.
     *
     * @details Midpoint = \f$(x_1+x_2)/2\f$, \f$(y_1+y_2)/2\f$
     */
    [[nodiscard]] constexpr Point<Type> GetMidPoint() const noexcept
    {
        auto const x = (start_.GetX() + end_.GetX()) / 2.0f;
        auto const y = (start_.GetY() + end_.GetY()) / 2.0f;
        return Point<Type> {x, y};
    }

    /**
     * @brief Returns the angle of the line in radians.
     */
    [[nodiscard]] Type GetAngleRadians() const noexcept
    {
        auto const angle = std::atan2(end_.GetY() - start_.GetY(), end_.GetX() - start_.GetX());
        return static_cast<Type>(angle);
    }

    /**
     * @brief Returns the angle of the line in degrees.
     */
    [[nodiscard]] Type GetAngleDegrees() const noexcept
    {
        return Type(GetAngleRadians() * (180.0 / 3.14159265358979323846));
    }

    /**
     * @brief Returns the length of the line.
     *
     * @details The distance between \f$(x_1,y_1)\f$ and \f$(x_2,y_2)\f$ is \f$\sqrt{(x_2-x_1)^2+(y_2-y_1)^2}\f$.
     */
    [[nodiscard]] Type GetLength() const noexcept
    {
        auto const x = std::pow(end_.GetX() - start_.GetX(), 2.0);
        auto const y = std::pow(end_.GetY() - start_.GetY(), 2.0);
        return Type(std::sqrt(x + y));
    }

    /**
     * @brief Prints the line in form of: ax + by = c
     */
    void PrintFormular() const
    {
        auto const a = end_.GetY() - start_.GetY();
        auto const b = start_.GetX() - end_.GetX();
        auto const c = a * (start_.GetX()) + b * (start_.GetY());

        if (b < 0)
        {
            fmt::print("The line passing through points P and Q is: {0}x {1}y = {2}\n", a, b, c);
        }
        else
        {
            fmt::print("The line passing through points P and Q is: {0}x + {1}y = {2}\n", a, b, c);
        }
    }

private:
    Point<Type> start_ {};
    Point<Type> end_ {};
};

template<typename Type>
bool operator==(Line<Type> const lhs, Line<Type> const rhs)
{
    auto const start = lhs.GetStart() == rhs.GetStart();
    auto const end   = lhs.GetEnd() == rhs.GetEnd();
    return (start && end);
}

template<typename Type>
bool operator!=(Line<Type> const lhs, Line<Type> const rhs)
{
    return !(lhs == rhs);
}

}  // namespace moci