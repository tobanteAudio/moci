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
    explicit PropertyMap(std::string name) : _name(std::move(name)) { }

    /**
     * @brief Sets the name.
     */
    auto setName(std::string newName) -> void { _name = std::move(newName); }

    /**
     * @brief Returns the name.
     */
    [[nodiscard]] auto getName() const noexcept -> std::string_view { return std::string_view(_name); }

    /**
     * @brief Sets or updates the property at key.
     */
    auto setProperty(KeyType const& key, Property&& p) -> void { _data.insert_or_assign(key, p); }

    /**
     * @brief Returns the property at key.
     */
    [[nodiscard]] auto getProperty(KeyType const& key) const noexcept -> std::optional<Property>
    {
        auto const search = _data.find(key);
        if (search != _data.end()) { return search->second; }

        return std::nullopt;
    }

    /**
     * @brief Delete the property at key. Returns true if successfully deleted.
     * If the key doesn't exist, false will be returned.
     */
    auto deleteProperty(KeyType const& key) -> bool { return _data.erase(key) == 1; }

    /**
     * @brief Returns true if the map contains the given key.
     */
    [[nodiscard]] auto hasKey(KeyType const& key) const -> bool
    {
        auto const search = _data.find(key);
        return search != _data.end();
    }

private:
    std::string _name {};
    std::unordered_map<KeyType, Property> _data {};
};
}  // namespace moci