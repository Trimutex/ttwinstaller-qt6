#pragma once

namespace TaleOfTwoWastelandsUI {
    // NOTE: public class
    // NOTE: interface class
	class IPrompts {
    public:
        std::string getFO3Path(void);
        std::string getFNVPath(void);
        std::string getTTWSavePath(void);

		bool buildPrompt(std::string name, std::string path);
        std::string fallout3Prompt(bool manual = false);
        std::string falloutNVPrompt(bool manual = false);
		bool overwritePrompt(std::string name, std::string path);
	    bool buildFOMODsPrompt();
		ErrorPromptResult patchingErrorPrompt(std::string file);
		void promptPaths();
        std::string ttwPrompt(bool manual = false);
    private:
        std::string m_fallout3Path;
		std::string m_falloutNVPath;
		std::string m_ttwSavePath;
	}
}
