#pragma once

namespace TaleOfTwoWastelands {
    // NOTE: public class
	class SettingsPathStore : IPathStore {
    public:
        string GetPathFromKey(string keyName);
		void SetPathFromKey(string keyName, string path);
	}
}
