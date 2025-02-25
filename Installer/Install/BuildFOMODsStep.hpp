#pragma once

#include <optional>

namespace TaleOfTwoWastelands.Install {
    class BuildFOMODsStep : IInstallStep {
    public:
        BuildFOMODsStep(IInstaller installer, ILog log, IPrompts prompts);

        std::optional<bool> Run(IInstallStatusUpdate status, CancellationToken token) {
            if (!_prompts.BuildFOMODsPrompt())
                return false;

            //+1 (opt)
            status.ItemsTotal++;
            status.CurrentOperation = "Building FOMODs";

            var fomodStatus = new InstallStatus(_installer.ProgressMinorOperation, _installer.Token);
            _fomod.BuildAll(fomodStatus, _installer.DirTTWMain, _installer.DirTTWOptional, _prompts.TTWSavePath);

            return true;
        }

    private:
        // TODO: read-only block
        IInstaller _m_installer;
        ILog m_log;
        IPrompts _m_prompts;
        FOMOD _m_fomod;
    };
}
