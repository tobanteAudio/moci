#include "moci/widgets/component/component.hpp"

#include "moci/core/preprocessor.hpp"

namespace moci
{

Component::Component(std::string id) : id_(std::move(id)) { }

void Component::onDraw(Painter& painter) { ignoreUnused(painter); }

void Component::onResize() { }

void Component::draw(Painter& painter)
{
    onDraw(painter);
    for (auto* child : children_) { child->draw(painter); }
}

void Component::resize()
{
    onResize();
    for (auto* child : children_) { child->onResize(); }
}

void Component::setId(std::string id) { id_ = std::move(id); }

auto Component::getId() const noexcept -> std::string_view { return id_; }

void Component::setParent(Component* p) { parent_ = p; }

auto Component::getParent() -> Component* { return parent_; }

auto Component::getRootComponent() -> Component*
{
    if (parent_ != nullptr) { return parent_->getRootComponent(); }
    return this;
}

void Component::addChild(Component* child)
{
    children_.push_back(child);
    child->setParent(this);
}

auto Component::getChildren() const noexcept -> Vector<Component*> const& { return children_; }

void Component::setPosition(int x, int y) noexcept
{
    bounds_.setPosition(x, y);
    resize();
}

void Component::setPosition(Point<int> position) noexcept
{
    bounds_.setPosition(position);
    resize();
}

auto Component::getX() const noexcept -> int { return bounds_.getX(); }

auto Component::getY() const noexcept -> int { return bounds_.getY(); }

void Component::setSize(int width, int height) noexcept
{
    bounds_.setWidth(width);
    bounds_.setHeight(height);
    resize();
}

auto Component::getWidth() const noexcept -> int { return bounds_.getWidth(); }

auto Component::getHeight() const noexcept -> int { return bounds_.getHeight(); }

void Component::setBounds(Rectangle<int> bounds) noexcept
{
    bounds_ = bounds;
    resize();
}

auto Component::getBounds() const noexcept -> Rectangle<int> { return bounds_; }

auto Component::contains(Point<int> position) const noexcept -> bool
{
    if (position.getX() >= getX() && position.getX() <= getX() + getWidth())
    {
        if (position.getY() >= getY() && position.getY() <= getY() + getHeight()) { return true; }
    }

    return false;
}

auto Component::findComponentAt(Point<int> position) noexcept -> Component*
{
    if (!contains(position)) { return nullptr; }

    for (auto* comp : getChildren())
    {
        if (comp->contains(position))
        {
            if (comp->getChildren().empty()) { return comp; }

            return comp->findComponentAt(position);
        }
    }

    return this;
}

void Component::setStyle(Style* newStyle) noexcept { style_ = newStyle; }

auto Component::getStyle() const noexcept -> Style*
{
    // If style is set local
    if (style_ != nullptr) { return style_; }

    // Style set on any parent
    if (parent_ != nullptr)
    {
        if (auto* parentStyle = parent_->getStyle(); parentStyle != nullptr) { return parentStyle; }
    }

    // No style set
    return nullptr;
}

}  // namespace moci