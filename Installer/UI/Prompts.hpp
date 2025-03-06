#pragma once

namespace TaleOfTwoWastelandsUI {
    // NOTE: public class
	class Prompts : IPrompts {
    public:
		string m_fallout3Path { get; private set; }
		string m_falloutNVPath { get; private set; }
		string m_ttwSavePath { get; private set; }

		Prompts(OpenFileDialog openDialog, SaveFileDialog saveDialog, ILog log, IEnumerable<IPathStore> store);
		void promptPaths();
		string fallout3Prompt(bool manual = false);
		string falloutNVPrompt(bool manual = false);
		string ttwPrompt(bool manual = false);
		bool overwritePrompt(string name, string path);
		bool buildPrompt(string name, string path);
	    bool buildFOMODsPrompt();
		ErrorPromptResult patchingErrorPrompt(string file);

    private:
        // NOTE: read-only block
		FileDialog m_openDialog, m_saveDialog;
		ILog m_log;
		IEnumerable<IPathStore> m_stores;

		string tryAllStoresGetPath(string key);
		bool tryAllStoresSetPath(string key, string path);
		string findByUserPrompt(FileDialog dialog, string name, string keyName, bool manual = false);
	}
}
