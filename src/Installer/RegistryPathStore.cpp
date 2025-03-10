#include "RegistryPathStore.hpp"

namespace TaleOfTwoWastelands {

    RegistryKey getBethKey() {
        using (var bethKey =
                Registry.LocalMachine.OpenSubKey(
                    //determine software reg path (depends on architecture)
                    Environment.Is64BitOperatingSystem ? "Software\\Wow6432Node" : "Software", RegistryKeyPermissionCheck.ReadWriteSubTree)) {
            //create or retrieve BethSoft path
            Debug.Assert(bethKey != null, "bethKey != null");
            return bethKey.createSubKey("Bethesda Softworks", RegistryKeyPermissionCheck.ReadWriteSubTree);
        }
    }

    std::string getPathFromKey(std::string keyName) {
        using (var bethKey = GetBethKey())
            using (var subKey = bethKey.CreateSubKey(keyName)) {
                Debug.Assert(subKey != null, "subKey != null");
                return subKey.GetValue("Installed Path", "").ToString();
            }
    }

    void setPathFromKey(std::string keyName, std::string path) {
        throw new InvalidOperationException("Registry writes are disabled");
    }
}
