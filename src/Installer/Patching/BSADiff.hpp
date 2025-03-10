#pragma once

#include <map>
#include <string>

// TODO: parallel defined here
namespace TaleOfTwoWastelandsPatching {
	using TaleOfTwoWastelands.Properties;
	using PatchJoin = Tuple<BSAFile, BSAFile, Patch>;
    // NOTE: public class
	class BsaDiff : IBsaDiff {
    public:
        BsaDiff(IInstaller installer, ILog log)
            : _m_installer(installer), m_log(log) {};

        bool patchBsa(CompressionOptions bsaOptions, std::string oldBSA, std::string newBSA, bool simulate = false)
        static IEnumerable<Tuple<std::string, std::string, std::string>> createRenameQuery(BSA bsa, std::map<std::string, std::string> renameDict);
        bool patchBsaFile(BSAFile bsaFile, PatchInfo patch, FileValidation targetChk);
        void renameFiles(BSA bsa, std::map<std::string, std::string> renameDict);

    protected:
        IProgress<InstallStatus> getProgress(void);
        CancellationToken getToken(void);

    private:
        // NOTE: read-only block
        ILog m_log;
		IInstaller _m_installer;

        private void handleFile(InstallStatus opChk, PatchJoin join)

    }
}
