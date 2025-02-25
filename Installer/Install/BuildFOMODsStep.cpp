#include "BuildFOMODsStep.hpp"

BuildFOMODsStep(IInstaller installer, ILog log, IPrompts prompts)
    : _m_installer(installer), m_log(log), _m_prompts(prompts) {
        _m_fomod = DependencyRegistry.Container.GetInstance<FOMOD>();
}
