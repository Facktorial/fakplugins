#include "../examples/RendererPlugin2.h"
#include "FakPlugins/Plugins.h"
#include "FakPlugins/PluginsInterfaces.h"
#include <spdlog/spdlog.h>

struct RendererPlugin3 : public RendererPlugin2
{
    virtual ~RendererPlugin3() = default;

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

    static constexpr std::string_view pluginName_ = "RendererPlugin2_2";
    static constexpr std::string_view pluginVersion_ = "0.0.2";
    static constexpr std::string_view pluginDescription_ = "Second dummy test plugin";
};

REGISTER_PLUGIN(RendererPlugin3)
