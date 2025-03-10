#include "BuildFOMODsStep.hpp"

BuildFOMODsStep(IInstaller installer, ILog log, IPrompts prompts)
    : _m_installer(installer), m_log(log), _m_prompts(prompts) {
        _m_fomod = DependencyRegistry.Container.GetInstance<FOMOD>();
    }

std::optional<bool> run(IInstallStatusUpdate status, CancellationToken token) {
    if (!_m_prompts.BuildFOMODsPrompt())
        return false;

    //+1 (opt)
    status.ItemsTotal++;
    status.CurrentOperation = "Building FOMODs";

    var fomodStatus = new InstallStatus(_m_installer.ProgressMinorOperation, _m_installer.Token);
    _m_fomod.BuildAll(fomodStatus, _m_installer.DirTTWMain, _m_installer.DirTTWOptional, _m_prompts.TTWSavePath);

    return true;
}
