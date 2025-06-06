#include "FakPlugins/Plugins.h"
#include "FakPlugins/PluginsInterfaces.h"
#include <spdlog/spdlog.h>

struct RendererPlugin : public FakPlugins::IPlugin
{
    virtual ~RendererPlugin() = default;

    FakPlugins::PluginInfo info_;

    [[nodiscard]] FakPlugins::PluginInfo getInfo() const final
    {
        return info_;
    }

    bool initialize() final
    {
        info_ = {pluginName_.data(), pluginVersion_.data(), pluginDescription_.data(), {}};
        spdlog::info("Hello: " + std::string(pluginName_));
        return true;
    }

    void shutdown() final
    {
        spdlog::info("Shutdown: " + std::string(pluginName_));
    }

    void execute(const std::function<void()> &func) final
    {
        spdlog::info("========================");
        spdlog::info("Executing " + std::string(pluginName_));
        spdlog::info("========================");

        if (func)
        {
            func();
        }

        spdlog::info("========================");
    }

    static constexpr std::string_view pluginName_ = "RendererPlugin";
    static constexpr std::string_view pluginVersion_ = "0.0.1";
    static constexpr std::string_view pluginDescription_ = "Dummy test plugin";
};

REGISTER_PLUGIN(RendererPlugin)
