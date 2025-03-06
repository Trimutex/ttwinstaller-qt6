#pragma once

namespace TaleOfTwoWastelandsUI {
    // NOTE: public class
    // NOTE: interface class
	class IPrompts {
		string m_fallout3Path { get; }
		string m_falloutNVPath { get; }
		string m_ttwSavePath { get; }

		bool buildPrompt(string name, string path);
		string fallout3Prompt(bool manual = false);
		string falloutNVPrompt(bool manual = false);
		bool overwritePrompt(string name, string path);
	    bool buildFOMODsPrompt();
		ErrorPromptResult patchingErrorPrompt(string file);
		void promptPaths();
		string ttwPrompt(bool manual = false);
	}
}
