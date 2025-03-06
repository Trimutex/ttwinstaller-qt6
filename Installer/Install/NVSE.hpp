#pragma once

#include <string>

namespace TaleOfTwoWastelandsInstall {
    // NOTE: originally internal
    class NVSE {
    public:
        NVSE(string FNVPath, ILog log) : _m_fnvPath(FNVPath), m_log(log) {};

        bool check(void);
        std::optional<bool> prompt(void);
        bool install(std::string& err);

    private:
        // TODO: read-only block
        std::string _m_fnvPath;
        ILog m_log;
    }
}
