#pragma once
#include "FakPlugins/PluginsInterfaces.h"
#include <spdlog/spdlog.h>

struct RendererPlugin2 : public FakPlugins::IPlugin
{
    virtual ~RendererPlugin2() = default;

    FakPlugins::PluginInfo info_;

    [[nodiscard]] FakPlugins::PluginInfo getInfo() const override
    {
        return info_;
    }

    bool initialize() override
    {
        info_ = {pluginName_.data(), pluginVersion_.data(), pluginDescription_.data(), {}};
        spdlog::info("Hello: " + std::string(pluginName_));
        return true;
    }

    void shutdown() override
    {
        spdlog::info("Shutdown: " + std::string(pluginName_));
    }

    void execute(const std::function<void()> &func) override
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

    static constexpr std::string_view pluginName_ = "RendererPlugin2";
    static constexpr std::string_view pluginVersion_ = "0.0.1";
    static constexpr std::string_view pluginDescription_ = "Second dummy test plugin";
};
