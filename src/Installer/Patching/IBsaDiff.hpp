#pragma once

#include <map>
#include <string>

namespace TaleOfTwoWastelandsPatching {
    // NOTE: public class
    // NOTE: interface class
	class IBsaDiff {
    public:
		bool patchBsa(CompressionOptions bsaOptions, std::string oldBSA, std::string newBSA, bool simulate = false);
		bool patchBsaFile(BSAFile bsaFile, PatchInfo patch, FileValidation targetChk);
		void renameFiles(BSA bsa, std::map<std::string, std::string> renameDict);
	}
}
