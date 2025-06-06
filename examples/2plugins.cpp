#include "../include/FakPlugins/Plugins.h"
#include <filesystem>
#include <iostream>
#include <numeric>
// #include <Fakstd/Utils/Common.h>

using Info = FakPlugins::PluginInfo;

int main()
{
    std::cout << "=== Modern C++ Plugin System Demo ===\n";

    FakPlugins::PluginManager manager;

    std::vector<std::string> pluginFiles = {"examples/rendererplugin0" PLUGIN_EXT, "examples/rendererplugin" PLUGIN_EXT,
                                            "examples/rendererplugin2" PLUGIN_EXT,
                                            "examples/rendererplugin3" PLUGIN_EXT};

    std::cout << "\n--- Loading Plugins ---\n";
    for (const auto &pluginFile : pluginFiles)
    {
        std::cout << pluginFile << '\n';

        if (std::filesystem::exists(pluginFile))
        {
            manager.loadPlugin(pluginFile);

            auto plugins = manager.getLoadedPluginNames();
            std::cout << std::accumulate(plugins.begin(), plugins.end(), std::string("Plugins: "),
                                         [](auto acc, auto plugin) { return acc + plugin + " "; })
                      << '\n';
        }
        else
        {
            std::cout << "Plugin file not found: " << pluginFile << '\n';
        }
    }

    std::cout << "\n--- Loaded Plugins ---\n";
    auto loadedPlugins = manager.getLoadedPluginNames();
    if (loadedPlugins.empty())
    {
        std::cout << "No plugins loaded.\n";
    }
    else
    {
        for (const auto &name : loadedPlugins)
        {
            auto m_helloPlugin = manager.getPlugin(name);
            if (m_helloPlugin.has_value())
            {
                auto *plugin = m_helloPlugin.value();
                Info info = plugin->getInfo();
                std::cout << name << " - " << info.name << " v" << info.version << std::endl;
            }
        }
        std::cout << "\n";
    }

    std::cout << "\n--- Executing Plugins ---\n";
    manager.executeAll();

    std::cout << "\n--- Individual Plugin Access ---\n";
    auto m_helloPlugin = manager.getPlugin("RendererPlugin");
    if (m_helloPlugin.has_value())
    {
        auto *plugin = m_helloPlugin.value();
        std::cout << "Accessing HelloPlugin individually:\n";
        plugin->execute(
            [&]()
            {
                // printType(m_helloPlugin);
                std::cout << "Accessed >>> " << plugin->getInfo().name << '\n';
            });
    }

    std::cout << "\n--- Unload Plugins ---\n";
    manager.unloadPlugin("RendererPlugin2");

    std::cout << "\n--- Loaded Plugins ---\n";
    loadedPlugins = manager.getLoadedPluginNames();
    if (loadedPlugins.empty())
    {
        std::cout << "No plugins loaded.\n";
    }
    else
    {
        for (const auto &name : loadedPlugins)
        {
            auto m_helloPlugin = manager.getPlugin(name);
            if (m_helloPlugin.has_value())
            {
                auto *plugin = m_helloPlugin.value();
                Info info = plugin->getInfo();
                std::cout << name << " - " << info.name << " v" << info.version << std::endl;
            }
        }
    }

    std::cout << "\n--- Unload Plugins ---\n";
    manager.unloadAllPlugins();

    std::cout << "\n--- Loaded Plugins ---\n";
    loadedPlugins = manager.getLoadedPluginNames();
    if (loadedPlugins.empty())
    {
        std::cout << "No plugins loaded.\n";
    }
    else
    {
        for (const auto &name : loadedPlugins)
        {
            auto m_helloPlugin = manager.getPlugin(name);
            if (m_helloPlugin.has_value())
            {
                auto *plugin = m_helloPlugin.value();
                Info info = plugin->getInfo();
                std::cout << name << " - " << info.name << " v" << info.version << std::endl;
            }
        }
        std::cout << "\n";
    }

    std::cout << "\n--- Demo Complete ---\n";
    return 0;
}
