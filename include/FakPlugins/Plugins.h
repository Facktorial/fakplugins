#pragma once
// #include <Fakstd/Utils/Common.h>
#include "FakPlugins/PluginsInterfaces.h"
#include <memory>
#include <optional>
#include <unordered_map>

#ifdef _WIN32
#include <windows.h>
#define PLUGIN_EXT ".dll"
using PluginHandle = HMODULE;
#else
#include <dlfcn.h>
#define PLUGIN_EXT ".so"
using PluginHandle = void *;
#endif

namespace FakPlugins
{
std::string some_function();

class PluginManager
{
    // FIXME:
    // template <IsPlugin Plugin>

    struct LoadedPlugin
    {
        PluginHandle handle;
        std::unique_ptr<IPlugin> plugin_instance;
        // CreatePluginFunc createFunc;
        // DestroyPluginFunc destroyFunc;

        LoadedPlugin() : handle(nullptr), plugin_instance(nullptr)
        {
        }

        LoadedPlugin(PluginHandle h, std::unique_ptr<IPlugin> instance)
            : handle(h), plugin_instance(std::move(instance))
        {
        }

        LoadedPlugin(LoadedPlugin &&other) noexcept
            : handle(other.handle), plugin_instance(std::move(other.plugin_instance))
        {
            // Leave other in valid state
            other.handle = nullptr;
        }

        LoadedPlugin &operator=(LoadedPlugin &&other) noexcept
        {
            if (this != &other)
            {
                cleanup();

                // Transfer ownership from other
                handle = other.handle;
                plugin_instance = std::move(other.plugin_instance);

                // Leave other in valid state
                other.handle = nullptr;
            }
            return *this;
        }

        LoadedPlugin(const LoadedPlugin &) = delete;
        LoadedPlugin &operator=(const LoadedPlugin &) = delete;

        ~LoadedPlugin()
        {
            cleanup();
        }

      private:
        void cleanup()
        {
            if (plugin_instance)
            {
                plugin_instance.reset();
            }

            if (handle)
            {
                closeLibrary(handle);
                handle = nullptr;
            }
        }
    };

    static PluginHandle loadLibrary(const std::string &filepath);
    static void *getSymbol(PluginHandle handle, const std::string &symbolName);
    static void closeLibrary(PluginHandle handle);
    static bool testFunc(PluginHandle handle, const auto &func, const std::string &funcName);

  public:
    ~PluginManager();

    bool loadPlugin(const std::string &pluginPath);
    bool unloadPlugin(const std::string &pluginName);
    void unloadAllPlugins();
    void executeAll();

    [[nodiscard]] std::optional<IPlugin *> getPlugin(const std::string &name) const;
    [[nodiscard]] std::vector<std::string> getLoadedPluginNames() const;

    std::unordered_map<std::string, LoadedPlugin> plugins_;
};

// Plugin factory function type
// The Problem: Cross-Boundary Memory Management
// When you load a plugin as a dynamic library (.dll/.so),
// you're crossing compilation boundaries.
// This creates several critical issues:
//
// * Different Memory Allocators
// * ABI Compatibility issues
// * Object Lifetime Management
//
// template <IsPlugin Plugin>
using CreatePluginFunc = std::unique_ptr<IPlugin> (*)();
// template <IsPlugin Plugin>
using DestroyPluginFunc = void (*)(IPlugin *);
} // namespace FakPlugins

#define REGISTER_PLUGIN(PluginName)                                                                                    \
    extern "C"                                                                                                         \
    {                                                                                                                  \
        std::unique_ptr<FakPlugins::IPlugin> createPlugin()                                                            \
        {                                                                                                              \
            return std::make_unique<PluginName>();                                                                     \
        }                                                                                                              \
        void destroyPlugin(FakPlugins::IPlugin *plugin)                                                                \
        {                                                                                                              \
            delete plugin;                                                                                             \
        }                                                                                                              \
        FakPlugins::PluginInfo getPluginInfo()                                                                         \
        {                                                                                                              \
            PluginName plugin;                                                                                         \
            plugin.initialize();                                                                                       \
            auto info = plugin.getInfo();                                                                              \
            return {info.name, info.version, #PluginName " Plugin", {}};                                               \
        }                                                                                                              \
    }
