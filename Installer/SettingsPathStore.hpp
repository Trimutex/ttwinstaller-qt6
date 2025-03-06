#pragma once

namespace TaleOfTwoWastelands {
    // NOTE: public class
	class SettingsPathStore : IPathStore {
    public:
        string getPathFromKey(string keyName);
		void setPathFromKey(string keyName, string path);
	}
}
