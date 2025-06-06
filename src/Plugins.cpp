#include <FakPlugins/Plugins.h>
#include <filesystem>
#include <memory>
#include <numeric>
#include <ranges>
#include <spdlog/spdlog.h>
#include <sstream>
#include <vector>

namespace FakPlugins
{

std::string some_function()
{
    auto vals = std::vector<int>{0, 1, 2, 3};
    return std::accumulate(vals.begin(), vals.end(), std::string(""),
                           [](auto acc, auto val) -> std::string { return acc + std::to_string(val); });
}

PluginManager::~PluginManager()
{
    unloadAllPlugins();
}

bool PluginManager::testFunc(PluginHandle handle, const auto &func, const std::string &funcName)
{
    if (func == nullptr)
    {
        spdlog::error("Plugin missing " + funcName + " function\n");
        PluginManager::closeLibrary(handle);
        return false;
    }
    return true;
}

bool PluginManager::loadPlugin(const std::string &pluginPath)
{
    try
    {
        PluginHandle handle = loadLibrary(pluginPath);
        if (handle == nullptr)
        {
            spdlog::error("Failed to load library: " + pluginPath + "\n");
            return false;
        }

        auto getInfoFunc = reinterpret_cast<PluginInfo (*)()>(getSymbol(handle, "getPluginInfo"));
        testFunc(handle, getInfoFunc, "getPluginInfo");

        // auto createFunc = reinterpret_cast<CreatePluginFunc<FakPlugins::IPlugin>>(getSymbol(handle, "createPlugin"));
        auto createFunc = reinterpret_cast<CreatePluginFunc>(getSymbol(handle, "createPlugin"));
        testFunc(handle, createFunc, "createPlugin");

        PluginInfo info = getInfoFunc();
        auto instance = createFunc();

        if (!instance)
        {
            spdlog::error("Failed to create plugin instance");
            closeLibrary(handle);
            return false;
        }

        if (!instance->initialize())
        {
            spdlog::error("Failed to initialize plugin instance");
            closeLibrary(handle);
            return false;
        }

        plugins_[info.name] = LoadedPlugin{handle, std::move(instance)};
        // plugins_[info.name] = LoadedPlugin {
        //   handle, std::make_unique<IPlugin>(std::move(instance))
        // };
        // plugins_.emplace({
        //   info.name, std::make_unique<IPlugin>(std::move(instance))
        // });

        spdlog::info("Loaded plugin: " + info.name + " v" + info.version);

        return true;
    }
    catch (const std::exception &e)
    {
        spdlog::error("Exception loading plugin: " + std::string(e.what()) + "\n");
        return false;
    }
    return true;
}

bool PluginManager::unloadPlugin(const std::string &pluginName)
{
    spdlog::warn("Plugin " + pluginName + " about to be removed!");
    auto it = plugins_.find(pluginName);
    if (it != plugins_.end())
    {
        plugins_.erase(it);
        // closeLibrary(it->second.handle);
        return true;
    }

    return false;
}

void PluginManager::unloadAllPlugins()
{
    // std::vector<PluginHandle> handles;
    // handles.reserve(plugins_.size());

    // for (auto& [plugin_name, plugin] : plugins_)
    // {
    //     handles.push_back(plugin.handle);
    // }
    plugins_.clear();
    // for (auto handle : handles)
    // {
    //   if (handle) { closeLibrary(handle); }
    // }
}

void PluginManager::executeAll()
{
    for (auto &[name, plugin] : plugins_)
    {
        spdlog::info("Executing " + name + "...");
        plugin.plugin_instance->execute([]() {});
    }
}

std::optional<IPlugin *> PluginManager::getPlugin(const std::string &name) const
{
    auto it = plugins_.find(name);
    if (it == plugins_.end())
    {
        return std::nullopt;
    }

    return std::optional<IPlugin *>{it->second.plugin_instance.get()};
}

std::vector<std::string> PluginManager::getLoadedPluginNames() const
{
    // using Item = std::pair<std::string, LoadedPlugin>;
    // auto& view = plugins_ | std::views::transform([](const Item& it_plugin) {
    //   return it_plugin.first;
    // });
    // return {view.begin(), view.end()};
    std::vector<std::string> names;
    names.reserve(plugins_.size());

    // auto keys = plugins_ | std::views::keys;
    // names.assign(keys.begin(), keys.end());
    for (const auto &[key, _] : plugins_)
    {
        names.push_back(key);
    }

    return names;
}

// Platform-specific implementations
PluginHandle PluginManager::loadLibrary(const std::string &filepath)
{
#ifdef _WIN32
    return LoadLibraryA(filepath.c_str());
#else
    return dlopen(filepath.c_str(), RTLD_LAZY);
#endif
}

void *PluginManager::getSymbol(PluginHandle handle, const std::string &symbolName)
{
#ifdef _WIN32
    return GetProcAddress(handle, symbolName.c_str());
#else
    return dlsym(handle, symbolName.c_str());
#endif
}

void PluginManager::closeLibrary(PluginHandle handle)
{
#ifdef _WIN32
    FreeLibrary(handle);
#else
    dlclose(handle);
#endif
}

} // namespace FakPlugins
