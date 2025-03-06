#pragma once

namespace TaleOfTwoWastelandsPatching {
    // NOTE: public class
    // NOTE: interface class
	class IBsaDiff {
    public:
		bool patchBsa(CompressionOptions bsaOptions, string oldBSA, string newBSA, bool simulate = false);
		bool patchBsaFile(BSAFile bsaFile, PatchInfo patch, FileValidation targetChk);
		void renameFiles(BSA bsa, IDictionary<string, string> renameDict);
	}
}
