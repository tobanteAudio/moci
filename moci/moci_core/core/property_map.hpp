#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <variant>

namespace moci
{

/**
 * @brief Container that maps keys to property objects.
 */
template<typename KeyType>
class PropertyMap
{
public:
    /**
     * @brief Property storage type.
     */
    using Property = std::variant<int, float, std::string>;

    /**
     * @brief Default constructor.
     */
    explicit PropertyMap() = default;

    /**
     * @brief Constructor. Sets the name.
     */
    explicit PropertyMap(std::string name) : name_(std::move(name)) { }

    /**
     * @brief Sets the name.
     */
    auto SetName(std::string newName) -> void { name_ = std::move(newName); }

    /**
     * @brief Returns the name.
     */
    [[nodiscard]] auto GetName() const noexcept -> std::string_view { return std::string_view(name_); }

    /**
     * @brief Sets or updates the property at key.
     */
    auto SetProperty(KeyType const& key, Property&& p) -> void { data_.insert_or_assign(key, p); }

    /**
     * @brief Returns the property at key.
     */
    [[nodiscard]] auto GetProperty(KeyType const& key) const noexcept -> std::optional<Property>
    {
        auto const search = data_.find(key);
        if (search != data_.end()) { return search->second; }

        return std::nullopt;
    }

    /**
     * @brief Delete the property at key. Returns true if successfully deleted.
     * If the key doesn't exist, false will be returned.
     */
    bool DeleteProperty(KeyType const& key) { return data_.erase(key) == 1; }

    /**
     * @brief Returns true if the map contains the given key.
     */
    [[nodiscard]] bool HasKey(KeyType const& key) const
    {
        auto const search = data_.find(key);
        return search != data_.end();
    }

private:
    std::string name_ {};
    std::unordered_map<KeyType, Property> data_ {};
};
}  // namespace moci