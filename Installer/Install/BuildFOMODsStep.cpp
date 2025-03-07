#include "BuildFOMODsStep.hpp"

BuildFOMODsStep(IInstaller installer, ILog log, IPrompts prompts)
    : _m_installer(installer), m_log(log), _m_prompts(prompts) {
        _m_fomod = DependencyRegistry.Container.GetInstance<FOMOD>();
    }

std::optional<bool> run(IInstallStatusUpdate status, CancellationToken token) {
    if (!_prompts.BuildFOMODsPrompt())
        return false;

    //+1 (opt)
    status.ItemsTotal++;
    status.CurrentOperation = "Building FOMODs";

    var fomodStatus = new InstallStatus(_installer.ProgressMinorOperation, _installer.Token);
    _fomod.BuildAll(fomodStatus, _installer.DirTTWMain, _installer.DirTTWOptional, _prompts.TTWSavePath);

    return true;
}
