#pragma once

#include "moci_math/geometry/point.hpp"

namespace moci
{
template<typename Type>
class Rectangle
{
public:
    /**
     * @brief Default construct. Top left at (0,0), no size.
     */
    explicit constexpr Rectangle() noexcept : position_(Type(0), Type(0)), width_(Type(0)), height_(Type(0)) { }

    /**
     * @brief Construct at (x, y), with width & height.
     */
    constexpr Rectangle(Type x, Type y, Type width, Type height) noexcept
        : position_(x, y), width_(width), height_(height)
    {
    }

    /**
     * @brief Construct at position, with width & height.
     */
    constexpr Rectangle(Point<Type> position, Type width, Type height) noexcept
        : position_(position), width_(width), height_(height)
    {
    }

    /**
     * @brief Default copy construct.
     */
    Rectangle(Rectangle const&) = default;

    /**
     * @brief Default copy assignment.
     */
    auto operator=(Rectangle const&) -> Rectangle& = default;

    /**
     * @brief Returns true if the rectangle has no width & no height.
     */
    [[nodiscard]] constexpr auto IsEmpty() const noexcept -> bool { return width_ <= Type(0) && height_ <= Type(0); }

    /**
     * @brief Set Top left to (x, y).
     */
    auto SetPosition(Type x, Type y) noexcept -> void { position_ = Point<Type>(x, y); }

    /**
     * @brief Set Top left to position.
     */
    auto SetPosition(Point<Type> position) noexcept -> void { position_ = position; }

    /**
     * @brief Set the x for the Top left corner.
     */
    auto SetX(Type x) noexcept -> void { position_.SetX(x); }

    /**
     * @brief Returns x off the left-hand side.
     */
    [[nodiscard]] constexpr auto GetX() const noexcept -> Type { return position_.GetX(); }

    /**
     * @brief Set the y for the Top left corner.
     */
    auto SetY(Type y) noexcept -> void { position_.SetY(y); }

    /**
     * @brief Returns x off the bottom side.
     */
    [[nodiscard]] constexpr auto GetY() const noexcept -> Type { return position_.GetY(); }

    /**
     * @brief Set the width.
     */
    auto SetWidth(Type width) noexcept -> void { width_ = width; }

    /**
     * @brief Returns the width.
     */
    [[nodiscard]] constexpr auto GetWidth() const noexcept -> Type { return width_; }

    /**
     * @brief Set the height.
     */
    auto SetHeight(Type height) noexcept -> void { height_ = height; }

    /**
     * @brief Returns the height.
     */
    [[nodiscard]] constexpr auto GetHeight() const noexcept -> Type { return height_; }

    /**
     * @brief Returns the x coordinate of the rectangle's right-side.
     */
    [[nodiscard]] constexpr auto GetRight() const noexcept -> Type { return position_.GetX() + width_; }

    /**
     * @brief Returns the y coordinate of the rectangle's bottom edge.
     */
    [[nodiscard]] constexpr auto GetBottom() const noexcept -> Type { return position_.GetY() + height_; }

    /**
     * @brief Returns the aspect ratio width / height.
     */
    [[nodiscard]] constexpr auto GetAspectRatio() const noexcept -> Type
    {
        if (GetWidth() != Type {})
        {
            return GetWidth() / GetHeight();
        }

        return Type {};
    }

    /**
     * @brief Returns a new rectangle from the top & shrinks the original.
     */
    [[nodiscard]] auto RemoveFromTop(Type amount) noexcept -> Rectangle<Type>
    {
        auto const oldY = GetY();
        SetY(oldY + amount);
        SetHeight(GetHeight() - amount);
        return Rectangle<Type> {GetX(), oldY, GetWidth(), amount};
    }

    /**
     * @brief Returns a new rectangle from the bottom & shrinks the original.
     */
    [[nodiscard]] auto RemoveFromBottom(Type amount) noexcept -> Rectangle<Type>
    {
        auto const oldHeight = GetHeight();
        auto const newY      = GetY() + oldHeight - amount;
        SetHeight(oldHeight - amount);
        return Rectangle<Type> {GetX(), newY, GetWidth(), amount};
    }

    /**
     * @brief Returns a new rectangle from the left & shrinks the original.
     */
    [[nodiscard]] auto RemoveFromLeft(Type amount) noexcept -> Rectangle<Type>
    {
        auto const oldX = GetX();
        SetWidth(GetWidth() - amount);
        SetX(oldX + amount);
        return Rectangle<Type> {oldX, GetY(), amount, GetHeight()};
    }

    /**
     * @brief Returns a new rectangle from the right & shrinks the original.
     */
    [[nodiscard]] auto RemoveFromRight(Type amount) noexcept -> Rectangle<Type>
    {
        auto const oldWidth = GetWidth();
        SetWidth(oldWidth - amount);
        return Rectangle<Type> {GetX() + oldWidth - amount, GetY(), amount, GetHeight()};
    }

    /**
     * @brief Returns a rectangle that is smaller than this one by a given amount.
     * Effectively, the rectangle returned is:
     * (x + deltaX, y + deltaY, w - deltaX * 2, h - deltaY * 2).
     */
    [[nodiscard]] auto Reduced(Type deltaX, Type deltaY) const noexcept -> Rectangle<Type>
    {
        return Rectangle<Type> {
            GetX() + deltaX,          //
            GetY() + deltaY,          //
            GetWidth() - deltaX * 2,  //
            GetHeight() - deltaY * 2  //
        };
    }

    /**
     * @brief Returns a rectangle that is smaller than this one by a given amount.
     * Effectively, the rectangle returned is:
     * (x + delta, y + delta, w - delta * 2, h - delta * 2).
     */
    [[nodiscard]] auto Reduced(Type delta) const noexcept -> Rectangle<Type> { return Reduced(delta, delta); }

    /**
     * @brief Converts the rectangle to a rectangle based on the given type.
     */
    template<typename ConvertType>
    [[nodiscard]] auto To() const noexcept -> Rectangle<ConvertType>
    {
        return {
            static_cast<ConvertType>(GetX()),       //
            static_cast<ConvertType>(GetY()),       //
            static_cast<ConvertType>(GetWidth()),   //
            static_cast<ConvertType>(GetHeight()),  //
        };
    }

    /**
     * @brief Converts the rectangle to a float version rectangle.
     */
    [[nodiscard]] auto ToFloat() const noexcept -> Rectangle<float>
    {
        return {
            static_cast<float>(GetX()),       //
            static_cast<float>(GetY()),       //
            static_cast<float>(GetWidth()),   //
            static_cast<float>(GetHeight()),  //
        };
    }

    /**
     * @brief Converts the rectangle to a int version rectangle.
     */
    [[nodiscard]] auto ToInt() const noexcept -> Rectangle<int>
    {
        return {
            static_cast<int>(GetX()),       //
            static_cast<int>(GetY()),       //
            static_cast<int>(GetWidth()),   //
            static_cast<int>(GetHeight()),  //
        };
    }

private:
    Point<Type> position_;
    Type width_, height_;
};

template<typename Type>
bool operator==(Rectangle<Type> const r1, Rectangle<Type> const r2)
{
    auto const position = (r1.GetX() == r2.GetX()) && (r1.GetY() == r2.GetY());
    auto const width    = r1.GetWidth() == r2.GetWidth();
    auto const height   = r1.GetHeight() == r2.GetHeight();
    return (position && width && height);
}

template<typename Type>
bool operator!=(Rectangle<Type> const r1, Rectangle<Type> const r2)
{
    return !(r1 == r2);
}

}  // namespace moci