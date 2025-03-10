#include "SettingsPathStore.hpp"

namespace TaleOfTwoWastelands {
    std::string getPathFromKey(std::string keyName) {
        return (std::string)Settings.Default[keyName];
    }

    void setPathFromKey(std::string keyName, std::string path) {
        Settings.Default[keyName] = path;
        Settings.Default.Save();
    }
}
