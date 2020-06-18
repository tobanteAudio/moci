#include "moci_widget/component/component.hpp"

#include "moci_core/core/preprocessor.hpp"

namespace moci
{

Component::Component(std::string id) : id_(std::move(id)) { }

void Component::OnDraw(Painter& painter) { IgnoreUnused(painter); }

void Component::OnResize() { }

void Component::Draw(Painter& painter)
{
    OnDraw(painter);
    for (auto* child : children_) { child->Draw(painter); }
}

void Component::Resize()
{
    OnResize();
    for (auto* child : children_) { child->OnResize(); }
}

void Component::SetID(std::string id) { id_ = std::move(id); }

auto Component::GetID() const noexcept -> std::string_view { return id_; }

void Component::SetParent(Component* p) { parent_ = p; }

auto Component::GetParent() -> Component* { return parent_; }

auto Component::GetRootComponent() -> Component*
{
    if (parent_ != nullptr) { return parent_->GetRootComponent(); }
    return this;
}

void Component::AddChild(Component* child)
{
    children_.push_back(child);
    child->SetParent(this);
}

auto Component::GetChildren() const noexcept -> Vector<Component*> const& { return children_; }

void Component::SetPosition(int x, int y) noexcept
{
    bounds_.SetPosition(x, y);
    Resize();
}

void Component::SetPosition(Point<int> position) noexcept
{
    bounds_.SetPosition(position);
    Resize();
}

auto Component::GetX() const noexcept -> int { return bounds_.GetX(); }

auto Component::GetY() const noexcept -> int { return bounds_.GetY(); }

void Component::SetSize(int width, int height) noexcept
{
    bounds_.SetWidth(width);
    bounds_.SetHeight(height);
    Resize();
}

auto Component::GetWidth() const noexcept -> int { return bounds_.GetWidth(); }

auto Component::GetHeight() const noexcept -> int { return bounds_.GetHeight(); }

void Component::SetBounds(Rectangle<int> bounds) noexcept
{
    bounds_ = bounds;
    Resize();
}

auto Component::GetBounds() const noexcept -> Rectangle<int> { return bounds_; }

auto Component::Contains(Point<int> position) const noexcept -> bool
{
    if (position.GetX() >= GetX() && position.GetX() <= GetX() + GetWidth())
    {
        if (position.GetY() >= GetY() && position.GetY() <= GetY() + GetHeight()) { return true; }
    }

    return false;
}

auto Component::FindComponentAt(Point<int> position) noexcept -> Component*
{
    if (!Contains(position)) { return nullptr; }

    for (auto* comp : GetChildren())
    {
        if (comp->Contains(position))
        {
            if (comp->GetChildren().empty()) { return comp; }

            return comp->FindComponentAt(position);
        }
    }

    return this;
}

void Component::SetStyle(Style* newStyle) noexcept { style_ = newStyle; }

Style* Component::GetStyle() const noexcept
{
    // If style is set local
    if (style_ != nullptr) { return style_; }

    // Style set on any parent
    if (parent_ != nullptr)
    {
        if (auto* parentStyle = parent_->GetStyle(); parentStyle != nullptr) { return parentStyle; }
    }

    // No style set
    return nullptr;
}

}  // namespace moci