#pragma once

#include <map>
#include <string>

namespace PatchMaker {
	using TaleOfTwoWastelands.Properties;
	using Patch = Tuple<FileValidation, PatchInfo[]>;

	class Program {
    public:
        static void main();
        static uint8_t getDiff(std::string diffPath, long convertSignature = -1, bool moveToUsed = false);

    private:
        const std::string m_inDir = "BuildDB";
        const std::string m_outDir = "OutDB";
        static std::string _m_dirTTWMain, _m_dirTTWOptional, _m_dirFO3Data;

        static void buildBsaPatch(std::string inBsaName, std::string outBsaName);
        static std::map<std::string, std::string> buildRenameDict(std::string bsaName);
        static void buildMasterPatch(std::string esm, ILookup<std::string, std::string> knownEsmVersions);
    }
}
