#pragma once

#include <string>

namespace TaleOfTwoWastelandsInstall {
    class FOMOD {
    public: 
        FOMOD(ILog log);
        void BuildAll(InstallStatus status, std::string mainBuildFolder, std::string optBuildFolder, std::string saveFolder);
        static void Build(InstallStatus status, std::string path, std::string fomod);

    private:
        // TODO: read-only block
        ILog m_log;
    }
}
