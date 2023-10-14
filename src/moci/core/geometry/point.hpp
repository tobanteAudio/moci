#pragma once

namespace moci
{
template<typename Type>
class Point
{
public:
    explicit constexpr Point() noexcept : _x(Type(0)), _y(Type(0)) { }
    constexpr Point(Type x, Type y) noexcept : _x(x), _y(y) { }

    ~Point() = default;

    Point(Point const&)                    = default;
    auto operator=(Point const&) -> Point& = default;

    Point(Point&&) noexcept                    = default;
    auto operator=(Point&&) noexcept -> Point& = default;

    [[nodiscard]] constexpr auto isOrigin() const noexcept -> bool { return _x == Type(0) && _y == Type(0); }

    [[nodiscard]] constexpr auto getX() const noexcept -> Type { return _x; }
    [[nodiscard]] constexpr auto getY() const noexcept -> Type { return _y; }

    auto setX(Type x) noexcept -> void { _x = x; }
    auto setY(Type y) noexcept -> void { _y = y; }

private:
    Type _x, _y;
};

template<typename Type>
constexpr auto operator==(Point<Type> lhs, Point<Type> rhs) -> bool
{
    return (lhs.getX() == rhs.getX()) && (lhs.getY() == rhs.getY());
}

template<typename Type>
constexpr auto operator!=(Point<Type> lhs, Point<Type> rhs) -> bool
{
    return !(lhs == rhs);
}

}  // namespace moci
