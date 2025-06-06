#pragma once
#include <concepts>
#include <functional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace FakPlugins
{
using Str = std::string;

struct PluginInfo
{
    Str name;
    Str version;
    Str description;
    std::vector<Str> dependencies;
};

struct IPlugin
{
    virtual ~IPlugin() = default;

    virtual void execute(const std::function<void()> &) = 0;
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;

    [[nodiscard]] virtual PluginInfo getInfo() const = 0;
};

// template<typename T>
// concept IsPlugin = std::is_base_of_v<IPlugin, T> &&
//                  !std::is_abstract_v<T>;
template <typename T>
concept IsPlugin = requires(T plugin) {
    std::destructible<T>;

    {
        plugin.initialize()
    } -> std::convertible_to<bool>;
    {
        plugin.shutdown()
    } -> std::same_as<void>;
    {
        plugin.execute(std::function<void()>{})
    } -> std::same_as<void>;

    {
        std::as_const(plugin).getInfo()
    } -> std::convertible_to<PluginInfo>;
};

// template<typename T>
// concept IsStrictPlugin = IsPlugin<T> &&
//     std::is_default_constructible_v<T> &&
//     std::movable<T> &&
//     // Must have virtual destructor (for polymorphic behavior)
//     std::has_virtual_destructor_v<T>;
} // namespace FakPlugins
