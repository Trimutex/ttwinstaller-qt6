#pragma once

namespace TaleOfTwoWastelands {
    // NOTE: public class
    class RegistryPathStore : IPathStore {
    public:
        RegistryKey getBethKey();
        string getPathFromKey(string keyName);
        void setPathFromKey(string keyName, string path);
    }
}
