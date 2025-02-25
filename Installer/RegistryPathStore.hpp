#pragma once

namespace TaleOfTwoWastelands {
    // NOTE: public class
    class RegistryPathStore : IPathStore {
    public:
        RegistryKey GetBethKey();
        string GetPathFromKey(string keyName);
        void SetPathFromKey(string keyName, string path);
    }
}
