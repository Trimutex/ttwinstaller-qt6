#pragma once

#include <string>

namespace TaleOfTwoWastelands {
    // NOTE: public class
    // NOTE: interface class
	class IPathStore {
    public:
        std::string getPathFromKey(std::string keyName);
		void setPathFromKey(std::string keyName, std::string path);
	}
}
