#pragma once
#include <optional>

namespace TaleOfTwoWastelandsInstall {
	class BuildBsasStep : IInstallStep {
    public:
		BuildBsasStep(IInstaller installer, IPrompts prompts) :
            _m_installer(installer), _m_prompts(prompts) {};
        std::optional<bool> run(IInstallStatusUpdate status, CancellationToken token);

    private:
		IInstaller _m_installer;
		IPrompts _m_prompts;
	}
}
