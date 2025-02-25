#pragma once

namespace TaleOfTwoWastelandsPatching {
    // NOTE: public class
    // NOTE: interface class
	class IBsaDiff {
    public:
		bool PatchBsa(CompressionOptions bsaOptions, string oldBSA, string newBSA, bool simulate = false);
		bool PatchBsaFile(BSAFile bsaFile, PatchInfo patch, FileValidation targetChk);
		void RenameFiles(BSA bsa, IDictionary<string, string> renameDict);
	}
}
