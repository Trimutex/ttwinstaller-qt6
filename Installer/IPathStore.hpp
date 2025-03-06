#pragma once

namespace TaleOfTwoWastelands {
    // NOTE: public class
    // NOTE: interface class
	class IPathStore {
    public:
		string getPathFromKey(string keyName);
		void setPathFromKey(string keyName, string path);
	}
}
