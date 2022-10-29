#pragma once

#include "moci/math/geometry/point.hpp"

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
    [[nodiscard]] constexpr auto isEmpty() const noexcept -> bool { return width_ <= Type(0) && height_ <= Type(0); }

    /**
     * @brief Set Top left to (x, y).
     */
    auto setPosition(Type x, Type y) noexcept -> void { position_ = Point<Type>(x, y); }

    /**
     * @brief Set Top left to position.
     */
    auto setPosition(Point<Type> position) noexcept -> void { position_ = position; }

    /**
     * @brief Set the x for the Top left corner.
     */
    auto setX(Type x) noexcept -> void { position_.setX(x); }

    /**
     * @brief Returns x off the left-hand side.
     */
    [[nodiscard]] constexpr auto getX() const noexcept -> Type { return position_.getX(); }

    /**
     * @brief Set the y for the Top left corner.
     */
    auto setY(Type y) noexcept -> void { position_.setY(y); }

    /**
     * @brief Returns x off the bottom side.
     */
    [[nodiscard]] constexpr auto getY() const noexcept -> Type { return position_.getY(); }

    /**
     * @brief Set the width.
     */
    auto setWidth(Type width) noexcept -> void { width_ = width; }

    /**
     * @brief Returns the width.
     */
    [[nodiscard]] constexpr auto getWidth() const noexcept -> Type { return width_; }

    /**
     * @brief Set the height.
     */
    auto setHeight(Type height) noexcept -> void { height_ = height; }

    /**
     * @brief Returns the height.
     */
    [[nodiscard]] constexpr auto getHeight() const noexcept -> Type { return height_; }

    /**
     * @brief Returns the x coordinate of the rectangle's right-side.
     */
    [[nodiscard]] constexpr auto getRight() const noexcept -> Type { return position_.getX() + width_; }

    /**
     * @brief Returns the y coordinate of the rectangle's bottom edge.
     */
    [[nodiscard]] constexpr auto getBottom() const noexcept -> Type { return position_.getY() + height_; }

    /**
     * @brief Returns the aspect ratio width / height.
     */
    [[nodiscard]] constexpr auto getAspectRatio() const noexcept -> Type
    {
        if (getWidth() != Type {}) { return getWidth() / getHeight(); }

        return Type {};
    }

    /**
     * @brief Returns a new rectangle from the top & shrinks the original.
     */
    [[nodiscard]] auto removeFromTop(Type amount) noexcept -> Rectangle<Type>
    {
        auto const oldY = getY();
        setY(oldY + amount);
        setHeight(getHeight() - amount);
        return Rectangle<Type> {getX(), oldY, getWidth(), amount};
    }

    /**
     * @brief Returns a new rectangle from the bottom & shrinks the original.
     */
    [[nodiscard]] auto removeFromBottom(Type amount) noexcept -> Rectangle<Type>
    {
        auto const oldHeight = getHeight();
        auto const newY      = getY() + oldHeight - amount;
        setHeight(oldHeight - amount);
        return Rectangle<Type> {getX(), newY, getWidth(), amount};
    }

    /**
     * @brief Returns a new rectangle from the left & shrinks the original.
     */
    [[nodiscard]] auto removeFromLeft(Type amount) noexcept -> Rectangle<Type>
    {
        auto const oldX = getX();
        setWidth(getWidth() - amount);
        setX(oldX + amount);
        return Rectangle<Type> {oldX, getY(), amount, getHeight()};
    }

    /**
     * @brief Returns a new rectangle from the right & shrinks the original.
     */
    [[nodiscard]] auto removeFromRight(Type amount) noexcept -> Rectangle<Type>
    {
        auto const oldWidth = getWidth();
        setWidth(oldWidth - amount);
        return Rectangle<Type> {getX() + oldWidth - amount, getY(), amount, getHeight()};
    }

    /**
     * @brief Returns a rectangle that is smaller than this one by a given amount.
     * Effectively, the rectangle returned is:
     * (x + deltaX, y + deltaY, w - deltaX * 2, h - deltaY * 2).
     */
    [[nodiscard]] auto reduced(Type deltaX, Type deltaY) const noexcept -> Rectangle<Type>
    {
        return Rectangle<Type> {
            getX() + deltaX,          //
            getY() + deltaY,          //
            getWidth() - deltaX * 2,  //
            getHeight() - deltaY * 2  //
        };
    }

    /**
     * @brief Returns a rectangle that is smaller than this one by a given amount.
     * Effectively, the rectangle returned is:
     * (x + delta, y + delta, w - delta * 2, h - delta * 2).
     */
    [[nodiscard]] auto reduced(Type delta) const noexcept -> Rectangle<Type> { return reduced(delta, delta); }

    /**
     * @brief Converts the rectangle to a rectangle based on the given type.
     */
    template<typename ConvertType>
    [[nodiscard]] auto to() const noexcept -> Rectangle<ConvertType>
    {
        return {
            static_cast<ConvertType>(getX()),       //
            static_cast<ConvertType>(getY()),       //
            static_cast<ConvertType>(getWidth()),   //
            static_cast<ConvertType>(getHeight()),  //
        };
    }

    /**
     * @brief Converts the rectangle to a float version rectangle.
     */
    [[nodiscard]] auto toFloat() const noexcept -> Rectangle<float>
    {
        return {
            static_cast<float>(getX()),       //
            static_cast<float>(getY()),       //
            static_cast<float>(getWidth()),   //
            static_cast<float>(getHeight()),  //
        };
    }

    /**
     * @brief Converts the rectangle to a int version rectangle.
     */
    [[nodiscard]] auto toInt() const noexcept -> Rectangle<int>
    {
        return {
            static_cast<int>(getX()),       //
            static_cast<int>(getY()),       //
            static_cast<int>(getWidth()),   //
            static_cast<int>(getHeight()),  //
        };
    }

private:
    Point<Type> position_;
    Type width_, height_;
};

template<typename Type>
auto operator==(Rectangle<Type> const r1, Rectangle<Type> const r2) -> bool
{
    auto const position = (r1.getX() == r2.getX()) && (r1.getY() == r2.getY());
    auto const width    = r1.getWidth() == r2.getWidth();
    auto const height   = r1.getHeight() == r2.getHeight();
    return (position && width && height);
}

template<typename Type>
auto operator!=(Rectangle<Type> const r1, Rectangle<Type> const r2) -> bool
{
    return !(r1 == r2);
}

}  // namespace moci