#pragma once

#include <string>

namespace TaleOfTwoWastelandsUI {
    // NOTE: public class
	class Prompts : IPrompts {
    public:
		Prompts(OpenFileDialog openDialog, SaveFileDialog saveDialog, ILog log, IEnumerable<IPathStore> store);
		void promptPaths();
        std::string getFO3Path(void);
        std::string getFNVPath(void);
        std::string getTTWSavePath(void);
        std::string fallout3Prompt(bool manual = false);
		std::string falloutNVPrompt(bool manual = false);
		std::string ttwPrompt(bool manual = false);
		bool overwritePrompt(std::string name, std::string path);
		bool buildPrompt(std:;string name, std::string path);
	    bool buildFOMODsPrompt();
		ErrorPromptResult patchingErrorPrompt(std::string file);

    private:
        // NOTE: read-only block
		FileDialog m_openDialog, m_saveDialog;
		ILog m_log;
		IEnumerable<IPathStore> m_stores;
        std::string m_fallout3Path;
        std::string m_falloutNVPath;
        std::string m_ttwSavePath;

        std::string tryAllStoresGetPath(std::string key);
		bool tryAllStoresSetPath(std::string key, string path);
        std::string findByUserPrompt(FileDialog dialog, std::string name, std::string keyName, bool manual = false);
	}
}
