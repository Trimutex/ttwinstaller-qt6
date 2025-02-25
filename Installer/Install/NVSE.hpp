#pragma once

#include <string>

namespace TaleOfTwoWastelandsInstall {
    // NOTE: originally internal
    class NVSE {
    public:
        NVSE(string FNVPath, ILog log) : _m_fnvPath(FNVPath), m_log(log) {};

        bool Check(void);
        std::optional<bool> Prompt(void);
        bool Install(std::string& err);

    private:
        // TODO: read-only block
        std::string _m_fnvPath;
        ILog m_log;
    }
}
