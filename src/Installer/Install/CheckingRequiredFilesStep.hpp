#pragma once

#include <optional>

namespace TaleOfTwoWastelandsInstall
{
	class CheckingRequiredFilesStep : IInstallStep {
    public:
		CheckingRequiredFilesStep(IInstaller installer, ILog log)
            : m_log(log), _m_installer(installer) {};

        std::optional<bool> run(IInstallStatusUpdate status, CancellationToken token);

    private:
        // TODO: read-only block
		IInstaller _m_installer;
		ILog m_log;


		bool checkFiles(void);
	}
}
