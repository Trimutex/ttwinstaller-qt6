#include "SettingsPathStore.hpp"

namespace TaleOfTwoWastelands {
    string getPathFromKey(string keyName) {
        return (string)Settings.Default[keyName];
    }

    void setPathFromKey(string keyName, string path) {
        Settings.Default[keyName] = path;
        Settings.Default.Save();
    }
}
