#pragma once

#include <string>

namespace TaleOfTwoWastelands {
    // NOTE: public class
    class RegistryPathStore : IPathStore {
    public:
        RegistryKey getBethKey();
        std::string getPathFromKey(std::string keyName);
        void setPathFromKey(std::string keyName, std::string path);
    }
}
