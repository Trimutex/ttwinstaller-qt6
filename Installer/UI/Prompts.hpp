#pragma once

namespace TaleOfTwoWastelandsUI {
    // NOTE: public class
	class Prompts : IPrompts {
    public:
		string Fallout3Path { get; private set; }
		string FalloutNVPath { get; private set; }
		string TTWSavePath { get; private set; }

		Prompts(OpenFileDialog openDialog, SaveFileDialog saveDialog, ILog log, IEnumerable<IPathStore> store);
		void PromptPaths();
		string Fallout3Prompt(bool manual = false);
		string FalloutNVPrompt(bool manual = false);
		string TTWPrompt(bool manual = false);
		bool OverwritePrompt(string name, string path);
		bool BuildPrompt(string name, string path);
	    bool BuildFOMODsPrompt();
		ErrorPromptResult PatchingErrorPrompt(string file);

    private:
        // NOTE: read-only block
		FileDialog openDialog, saveDialog;
		ILog Log;
		IEnumerable<IPathStore> Stores;

		string TryAllStoresGetPath(string key);
		bool TryAllStoresSetPath(string key, string path);
		string FindByUserPrompt(FileDialog dialog, string name, string keyName, bool manual = false);
	}
}
