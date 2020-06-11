#pragma once

#include "moci_graphics/render_queue.hpp"
#include "moci_math/geometry/rectangle.hpp"

#include "moci_core/core/vector.hpp"
#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace moci
{

class Style;

/**
 * @brief Base for all GUI components.
 */
class Component
{
public:
    /**
     * @brief Default construct. At (0,0), no size & no parent
     */
    explicit Component() = default;

    /**
     * @brief Default construct and set id.
     */
    Component(std::string id);

    /**
     * @brief Virtual destructor.
     */
    virtual ~Component() = default;

    /**
     * @brief Callback for drawing.
     */
    virtual void OnDraw(RenderQueue& painter);

    /**
     * @brief Callback for layout changes.
     */
    virtual void OnResize();

    /**
     * @brief Draw should only be called on the root component.
     * It calls the OnDraw() callback on  all children.
     */
    auto Draw(RenderQueue& painter) -> void;

    /**
     * @brief Resize should only be called on the root component.
     * It calls the OnResized() callback on all children.
     */
    auto Resize() -> void;

    /**
     * @brief Set the components identifier string.
     */
    void SetID(std::string id);

    /**
     * @brief Returns the components identifier string.
     */
    [[nodiscard]] auto GetID() const noexcept -> std::string_view;

    /**
     * @brief Sets the direct parent.
     */
    void SetParent(Component* p);

    /**
     * @brief Gets the direct parent.
     */
    auto GetParent() -> Component*;

    /**
     * @brief Sets the root parent.
     */
    auto GetRootComponent() -> Component*;

    /**
     * @brief Adds the given compoonent as a child.
     */
    auto AddChild(Component* child) -> void;

    /**
     * @brief Returns a vector of all children.
     */
    [[nodiscard]] auto GetChildren() const noexcept -> Vector<Component*> const&;

    /**
     * @brief Sets the bottom left as (x, y).
     */
    void SetPosition(int x, int y) noexcept;

    /**
     * @brief Sets the bottom left as position.
     */
    void SetPosition(Point<int> position) noexcept;

    /**
     * @brief Returns a the x off the left edge.
     */
    [[nodiscard]] auto GetX() const noexcept -> int;

    /**
     * @brief Returns a the y off the bottom edge.
     */
    [[nodiscard]] auto GetY() const noexcept -> int;

    /**
     * @brief Sets the size.
     */
    void SetSize(int width, int height) noexcept;

    /**
     * @brief Returns a the width.
     */
    [[nodiscard]] auto GetWidth() const noexcept -> int;

    /**
     * @brief Returns a the height.
     */
    [[nodiscard]] auto GetHeight() const noexcept -> int;

    /**
     * @brief Sets the bounds to the given rectangle.
     */
    void SetBounds(Rectangle<int> bounds) noexcept;

    /**
     * @brief Returns a the global bounds.
     */
    [[nodiscard]] auto GetBounds() const noexcept -> Rectangle<int>;

    /**
     * @brief Returns true, if the given position is inside this component or it's children.
     */
    [[nodiscard]] auto Contains(Point<int> position) const noexcept -> bool;

    /**
     * @brief Returns the component at the position.
     */
    [[nodiscard]] auto FindComponentAt(Point<int> position) noexcept -> Component*;

    /**
     * @brief Sets the style for this component.
     */
    void SetStyle(Style* newStyle) noexcept;

    /**
     * @brief Returns the current set style.
     */
    [[nodiscard]] Style* GetStyle() const noexcept;

private:
    std::string id_ {};
    Component* parent_ {nullptr};
    Vector<Component*> children_ {};
    Rectangle<int> bounds_ {};
    Style* style_ {nullptr};
};

}  // namespace moci