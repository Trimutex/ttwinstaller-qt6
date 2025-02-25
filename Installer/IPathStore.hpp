#pragma once

namespace TaleOfTwoWastelands {
    // NOTE: public class
    // NOTE: interface class
	class IPathStore {
		string GetPathFromKey(string keyName);
		void SetPathFromKey(string keyName, string path);
	}
}
