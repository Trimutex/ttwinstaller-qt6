#pragma once

namespace TaleOfTwoWastelandsProperties {
	using IDSS = IDictionary<string, string>;
	using IDSSA = IDictionary<string, string[]>;

    // NOTE: internal class
    // NOTE: partial class
	class Game {
    public:
        static IDSS getBuildableBSAs();
        static IDSSA getCheckedBSAs();
        static std::string[] getCheckedESMs();
        static IDSS getVoicePaths();

    private:
		static Lazy<IDSS> _m_voicePaths = new Lazy<IDSS>(() => JsonConvert.DeserializeObject<IDSS>(ResourceManager.GetString("VoicePaths", resourceCulture)));
		static Lazy<string[]> _m_checkedEsms = new Lazy<string[]>(() => JsonConvert.DeserializeObject<string[]>(ResourceManager.GetString("CheckedESMs", resourceCulture)));
		static Lazy<IDSSA> _m_checkedBsas = new Lazy<IDSSA>(() => JsonConvert.DeserializeObject<IDSSA>(ResourceManager.GetString("CheckedBSAs", resourceCulture)));
		static Lazy<IDSS> _m_buildableBsas = new Lazy<IDSS>(() => JsonConvert.DeserializeObject<IDSS>(ResourceManager.GetString("BuildableBSAs", resourceCulture)));
	}
}
