#pragma once

#include <optional>

namespace TaleOfTwoWastelands.Install {
    class BuildFOMODsStep : IInstallStep {
    public:
        BuildFOMODsStep(IInstaller installer, ILog log, IPrompts prompts);
        std::optional<bool> run(IInstallStatusUpdate status, CancellationToken token);

    private:
        // TODO: read-only block
        IInstaller _m_installer;
        ILog m_log;
        IPrompts _m_prompts;
        FOMOD _m_fomod;
    };
}
