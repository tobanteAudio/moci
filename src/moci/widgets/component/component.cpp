#include "component.hpp"

#include <moci/core/preprocessor.hpp>

namespace moci {

Component::Component(std::string id) : _id(std::move(id)) {}

void Component::onDraw(Painter& painter) { ignoreUnused(painter); }

void Component::onResize() {}

void Component::draw(Painter& painter)
{
    onDraw(painter);
    for (auto* child : _children) {
        child->draw(painter);
    }
}

void Component::resize()
{
    onResize();
    for (auto* child : _children) {
        child->onResize();
    }
}

void Component::setId(std::string id) { _id = std::move(id); }

auto Component::getId() const noexcept -> std::string_view { return _id; }

void Component::setParent(Component* p) { _parent = p; }

auto Component::getParent() -> Component* { return _parent; }

auto Component::getRootComponent() -> Component*
{
    if (_parent != nullptr) {
        return _parent->getRootComponent();
    }
    return this;
}

void Component::addChild(Component* child)
{
    _children.push_back(child);
    child->setParent(this);
}

auto Component::getChildren() const noexcept -> std::vector<Component*> const&
{
    return _children;
}

void Component::setPosition(int x, int y) noexcept
{
    _bounds.setPosition(x, y);
    resize();
}

void Component::setPosition(Point<int> position) noexcept
{
    _bounds.setPosition(position);
    resize();
}

auto Component::getX() const noexcept -> int { return _bounds.getX(); }

auto Component::getY() const noexcept -> int { return _bounds.getY(); }

void Component::setSize(int width, int height) noexcept
{
    _bounds.setWidth(width);
    _bounds.setHeight(height);
    resize();
}

auto Component::getWidth() const noexcept -> int { return _bounds.getWidth(); }

auto Component::getHeight() const noexcept -> int { return _bounds.getHeight(); }

void Component::setBounds(Rectangle<int> bounds) noexcept
{
    _bounds = bounds;
    resize();
}

auto Component::getBounds() const noexcept -> Rectangle<int> { return _bounds; }

auto Component::contains(Point<int> position) const noexcept -> bool
{
    if (position.getX() >= getX() && position.getX() <= getX() + getWidth()) {
        if (position.getY() >= getY() && position.getY() <= getY() + getHeight()) {
            return true;
        }
    }

    return false;
}

auto Component::findComponentAt(Point<int> position) noexcept -> Component*
{
    if (!contains(position)) {
        return nullptr;
    }

    for (auto* comp : getChildren()) {
        if (comp->contains(position)) {
            if (comp->getChildren().empty()) {
                return comp;
            }

            return comp->findComponentAt(position);
        }
    }

    return this;
}

void Component::setStyle(Style* newStyle) noexcept { _style = newStyle; }

auto Component::getStyle() const noexcept -> Style*
{
    // If style is set local
    if (_style != nullptr) {
        return _style;
    }

    // Style set on any parent
    if (_parent != nullptr) {
        if (auto* parentStyle = _parent->getStyle(); parentStyle != nullptr) {
            return parentStyle;
        }
    }

    // No style set
    return nullptr;
}

}  // namespace moci
