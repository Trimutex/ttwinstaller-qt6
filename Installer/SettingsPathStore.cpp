#include "SettingsPathStore.hpp"

namespace TaleOfTwoWastelands {
    string GetPathFromKey(string keyName) {
        return (string)Settings.Default[keyName];
    }

    void SetPathFromKey(string keyName, string path) {
        Settings.Default[keyName] = path;
        Settings.Default.Save();
    }
}
