#pragma once

namespace PatchMaker {
	using TaleOfTwoWastelands.Properties;
	using Patch = Tuple<FileValidation, PatchInfo[]>;

	class Program {
    public:
        static void Main();
        static byte[] GetDiff(string diffPath, long convertSignature = -1, bool moveToUsed = false);

    private:
        const string InDir = "BuildDB";
        const string OutDir = "OutDB";
        static string _dirTTWMain, _dirTTWOptional, _dirFO3Data;

        static void BuildBsaPatch(string inBsaName, string outBsaName);
        static IDictionary<string, string> BuildRenameDict(string bsaName);
        static void BuildMasterPatch(string esm, ILookup<string, string> knownEsmVersions);
    }
}
