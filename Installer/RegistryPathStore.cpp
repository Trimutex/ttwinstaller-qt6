#include "RegistryPathStore.hpp"

namespace TaleOfTwoWastelands {

    RegistryKey GetBethKey() {
        using (var bethKey =
                Registry.LocalMachine.OpenSubKey(
                    //determine software reg path (depends on architecture)
                    Environment.Is64BitOperatingSystem ? "Software\\Wow6432Node" : "Software", RegistryKeyPermissionCheck.ReadWriteSubTree)) {
            //create or retrieve BethSoft path
            Debug.Assert(bethKey != null, "bethKey != null");
            return bethKey.CreateSubKey("Bethesda Softworks", RegistryKeyPermissionCheck.ReadWriteSubTree);
        }
    }

    string GetPathFromKey(string keyName) {
        using (var bethKey = GetBethKey())
            using (var subKey = bethKey.CreateSubKey(keyName)) {
                Debug.Assert(subKey != null, "subKey != null");
                return subKey.GetValue("Installed Path", "").ToString();
            }
    }

    void SetPathFromKey(string keyName, string path) {
        throw new InvalidOperationException("Registry writes are disabled");
    }
}
