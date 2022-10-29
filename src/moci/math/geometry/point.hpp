#pragma once

namespace moci
{
template<typename Type>
class Point
{
public:
    explicit constexpr Point() noexcept : x_(Type(0)), y_(Type(0)) { }
    constexpr Point(Type x, Type y) noexcept : x_(x), y_(y) { }

    ~Point() = default;

    Point(Point const&)                    = default;
    auto operator=(Point const&) -> Point& = default;

    Point(Point&&) noexcept                    = default;
    auto operator=(Point&&) noexcept -> Point& = default;

    [[nodiscard]] constexpr auto isOrigin() const noexcept -> bool { return x_ == Type(0) && y_ == Type(0); }

    [[nodiscard]] constexpr auto getX() const noexcept -> Type { return x_; }
    [[nodiscard]] constexpr auto getY() const noexcept -> Type { return y_; }

    auto setX(Type x) noexcept -> void { x_ = x; }
    auto setY(Type y) noexcept -> void { y_ = y; }

private:
    Type x_, y_;
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