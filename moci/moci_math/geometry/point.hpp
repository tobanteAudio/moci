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

    [[nodiscard]] constexpr auto IsOrigin() const noexcept -> bool { return x_ == Type(0) && y_ == Type(0); }

    [[nodiscard]] constexpr auto GetX() const noexcept -> Type { return x_; }
    [[nodiscard]] constexpr auto GetY() const noexcept -> Type { return y_; }

    auto SetX(Type x) noexcept -> void { x_ = x; }
    auto SetY(Type y) noexcept -> void { y_ = y; }

private:
    Type x_, y_;
};

template<typename Type>
constexpr bool operator==(Point<Type> lhs, Point<Type> rhs)
{
    return (lhs.GetX() == rhs.GetX()) && (lhs.GetY() == rhs.GetY());
}

template<typename Type>
constexpr bool operator!=(Point<Type> lhs, Point<Type> rhs)
{
    return !(lhs == rhs);
}

}  // namespace moci