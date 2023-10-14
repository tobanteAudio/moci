#pragma once

#include <moci/core/geometry/rectangle.hpp>
#include <moci/events/types/event.hpp>
#include <moci/events/types/mouse_event.hpp>
#include <moci/graphics_extra/painter.hpp>

#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace moci
{

class Style;

/**
 * @brief Base for all GUI components.
 */
class Component : public MouseCallback
{
public:
    /**
     * @brief Default construct. At (0,0), no size & no parent
     */
    explicit Component() = default;

    /**
     * @brief Default construct and set id.
     */
    explicit Component(std::string id);

    /**
     * @brief Virtual destructor.
     */
    ~Component() override = default;

    /**
     * @brief Callback for drawing.
     */
    virtual void onDraw(Painter& painter);

    /**
     * @brief Callback for layout changes.
     */
    virtual void onResize();

    /**
     * @brief Draw should only be called on the root component.
     * It calls the OnDraw() callback on  all children.
     */
    auto draw(Painter& painter) -> void;

    /**
     * @brief Resize should only be called on the root component.
     * It calls the OnResized() callback on all children.
     */
    auto resize() -> void;

    /**
     * @brief Set the components identifier string.
     */
    void setId(std::string id);

    /**
     * @brief Returns the components identifier string.
     */
    [[nodiscard]] auto getId() const noexcept -> std::string_view;

    /**
     * @brief Sets the direct parent.
     */
    void setParent(Component* p);

    /**
     * @brief Gets the direct parent.
     */
    auto getParent() -> Component*;

    /**
     * @brief Sets the root parent.
     */
    auto getRootComponent() -> Component*;

    /**
     * @brief Adds the given compoonent as a child.
     */
    auto addChild(Component* child) -> void;

    /**
     * @brief Returns a vector of all children.
     */
    [[nodiscard]] auto getChildren() const noexcept -> std::vector<Component*> const&;

    /**
     * @brief Sets the bottom left as (x, y).
     */
    void setPosition(int x, int y) noexcept;

    /**
     * @brief Sets the bottom left as position.
     */
    void setPosition(Point<int> position) noexcept;

    /**
     * @brief Returns a the x off the left edge.
     */
    [[nodiscard]] auto getX() const noexcept -> int;

    /**
     * @brief Returns a the y off the bottom edge.
     */
    [[nodiscard]] auto getY() const noexcept -> int;

    /**
     * @brief Sets the size.
     */
    void setSize(int width, int height) noexcept;

    /**
     * @brief Returns a the width.
     */
    [[nodiscard]] auto getWidth() const noexcept -> int;

    /**
     * @brief Returns a the height.
     */
    [[nodiscard]] auto getHeight() const noexcept -> int;

    /**
     * @brief Sets the bounds to the given rectangle.
     */
    void setBounds(Rectangle<int> bounds) noexcept;

    /**
     * @brief Returns a the global bounds.
     */
    [[nodiscard]] auto getBounds() const noexcept -> Rectangle<int>;

    /**
     * @brief Returns true, if the given position is inside this component or it's children.
     */
    [[nodiscard]] auto contains(Point<int> position) const noexcept -> bool;

    /**
     * @brief Returns the component at the position.
     */
    [[nodiscard]] auto findComponentAt(Point<int> position) noexcept -> Component*;

    /**
     * @brief Sets the style for this component.
     */
    void setStyle(Style* newStyle) noexcept;

    /**
     * @brief Returns the current set style.
     */
    [[nodiscard]] auto getStyle() const noexcept -> Style*;

private:
    std::string _id {};
    Component* _parent {nullptr};
    std::vector<Component*> _children {};
    Rectangle<int> _bounds {};
    Style* _style {nullptr};
};

}  // namespace moci