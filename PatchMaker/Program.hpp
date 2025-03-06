#pragma once

namespace PatchMaker {
	using TaleOfTwoWastelands.Properties;
	using Patch = Tuple<FileValidation, PatchInfo[]>;

	class Program {
    public:
        static void main();
        static byte[] getDiff(string diffPath, long convertSignature = -1, bool moveToUsed = false);

    private:
        const string m_inDir = "BuildDB";
        const string m_outDir = "OutDB";
        static string _m_dirTTWMain, _m_dirTTWOptional, _m_dirFO3Data;

        static void buildBsaPatch(string inBsaName, string outBsaName);
        static IDictionary<string, string> buildRenameDict(string bsaName);
        static void buildMasterPatch(string esm, ILookup<string, string> knownEsmVersions);
    }
}
