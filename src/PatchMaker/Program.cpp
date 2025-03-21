﻿#include "Program.hpp"

namespace PatchMaker {
    // NOTE: MAIN IS HERE
    static void main() {
        //BenchmarkHash.Run();

        if (!Debugger.IsAttached)
            Debugger.Launch();

        Console.WriteLine("Building {0} folder from {1} folder. Existing files are skipped. OK?", OutDir, InDir);
        Console.Write("y/n: ");
        var keyInfo = Console.ReadKey();
        switch (keyInfo.Key) {
            case ConsoleKey.Y:
                Console.WriteLine();
                break;
            default:
                return;
        }

        Directory.CreateDirectory(OutDir);

        _dirTTWMain = Path.Combine(InDir, "Main Files");
        _dirTTWOptional = Path.Combine(InDir, "Optional Files");

        var helper = new RegistryPathStore();
        var bethKey = helper.GetBethKey();

        var fo3Key = bethKey.CreateSubKey("Fallout3");
        Debug.Assert(fo3Key != null, "fo3Key != null");

        var fallout3Path = fo3Key.GetValue("Installed Path", "").ToString();
        _dirFO3Data = Path.Combine(fallout3Path, "Data");

        SevenZipCompressor.LzmaDictionarySize = 1024 * 1024 * 64; //64MiB, 7z 'Ultra'

        Parallel.ForEach(Game.BuildableBSAs, new ParallelOptions { MaxDegreeOfParallelism = 2 }, kvpBsa => BuildBsaPatch(kvpBsa.Key, kvpBsa.Value));

        var knownEsmVersions =
            Directory.EnumerateFiles(Path.Combine(InDir, "Versions"), "*.esm", SearchOption.AllDirectories)
            .ToLookup(Path.GetFileName, esm => esm);

        Parallel.ForEach(Game.CheckedESMs, new ParallelOptions { MaxDegreeOfParallelism = 2 }, esm => BuildMasterPatch(esm, knownEsmVersions));
    }

    // NOTE: originally unsafe
    static byte[] getDiff(std::string diffPath, long convertSignature = -1, bool moveToUsed = false) {
        if (File.Exists(diffPath)) {
            try {
                var diffBytes = File.ReadAllBytes(diffPath);
                if (convertSignature > 0)
                    fixed (byte* pBz2 = diffBytes)
                        return MakeDiff.ConvertPatch(pBz2, diffBytes.Length, Diff.SIG_BSDIFF40, convertSignature);

                return diffBytes;
            } catch ( ... ) { }
            if (moveToUsed)
                File.Move(diffPath, Path.ChangeExtension(diffPath, ".used"));
        }

        return null;
    }

    static void buildBsaPatch(std::string inBsaName, std::string outBsaName) {
        std::string outBSAFile = Path.ChangeExtension(outBsaName, ".bsa");
        std::string outBSAPath = Path.Combine(_dirTTWMain, outBSAFile);

        std::string inBSAFile = Path.ChangeExtension(inBsaName, ".bsa");
        std::string inBSAPath = Path.Combine(_dirFO3Data, inBSAFile);

        var renameDict = BuildRenameDict(outBsaName);
        Debug.Assert(renameDict != null);

        var patPath = Path.Combine(OutDir, Path.ChangeExtension(outBsaName, ".pat"));
        if (File.Exists(patPath))
            return;

        var prefix = Path.Combine(InDir, "TTW Patches", outBsaName);

        using (var inBSA = new BSA(inBSAPath))
            using (var outBSA = new BSA(outBSAPath)) {
                BsaDiff
                    .CreateRenameQuery(inBSA, renameDict)
                    .ToList(); // execute query

                var oldFiles = inBSA.SelectMany(folder => folder).ToList();
                var newFiles = outBSA.SelectMany(folder => folder).ToList();

                var newChkDict = FileValidation.FromBSA(outBSA);

                var joinedPatches = from patKvp in newChkDict
                    join newBsaFile in newFiles on patKvp.Key equals newBsaFile.Filename
                    select new
                    {
                        newBsaFile,
                            file = patKvp.Key,
                            patch = patKvp.Value,
                    };
                var allJoinedPatches = joinedPatches.ToList();

                var patchDict = new PatchDict(allJoinedPatches.Count);
                foreach (var join in allJoinedPatches) {
                    var oldBsaFile = oldFiles.SingleOrDefault(file => file.Filename == join.file);
                    Debug.Assert(oldBsaFile != null, "File not found: " + join.file);

                    var oldChk = FileValidation.FromBSAFile(oldBsaFile);
                    var newChk = join.patch;

                    var oldFilename = oldBsaFile.Filename;
                    if (oldFilename.StartsWith(Game.VoicePrefix)) {
                        patchDict.Add(join.file, new Patch(newChk, null));
                        continue;
                    }

                    var patches = new List<PatchInfo>();

                    var md5OldStr = Util.GetMD5String(oldBsaFile.GetContents(true));
                    var md5NewStr = Util.GetMD5String(join.newBsaFile.GetContents(true));

                    var diffPath = Path.Combine(prefix, oldFilename + "." + md5OldStr + "." + md5NewStr + ".diff");
                    var usedPath = Path.ChangeExtension(diffPath, ".used");
                    if (File.Exists(usedPath))
                        File.Move(usedPath, diffPath); //fixes moronic things

                    var altDiffs = Util.FindAlternateVersions(diffPath);
                    if (altDiffs != null) {
                        foreach (var altDiff in altDiffs) {
                            var altDiffBytes = GetDiff(altDiff.Item1, Diff.SIG_LZDIFF41);
                            patches.Add(new PatchInfo {
                                    Metadata = new FileValidation(altDiff.Item2, 0, FileValidation.ChecksumType.Md5),
                                    Data = altDiffBytes
                                    });
                        }
                    }

                    if (newChk != oldChk) {
                        byte[] diffData = GetDiff(diffPath, Diff.SIG_LZDIFF41);

                        var patchInfo = PatchInfo.FromOldDiff(diffData, oldChk);
                        Debug.Assert(patchInfo.Data != null);

                        patches.Add(patchInfo);
                    }

                    patchDict.Add(join.file, new Patch(newChk, patches.ToArray()));
                }

                using (var stream = File.OpenWrite(patPath))
                    patchDict.WriteAll(stream);
            }
    }

    static std::map<std::string, std::string> buildRenameDict(std::string bsaName) {
        var dictPath = Path.Combine(InDir, "TTW Patches", bsaName, "RenameFiles.dict");
        if (File.Exists(dictPath)) {
            var renameDict = Util.ReadOldDatabase(dictPath);
            var newRenPath = Path.Combine(OutDir, Path.ChangeExtension(bsaName, ".ren"));
            if (!File.Exists(newRenPath))
                using (var fileStream = File.OpenWrite(newRenPath))
                    using (var lzmaStream = new LzmaEncodeStream(fileStream))
                    using (var writer = new BinaryWriter(lzmaStream)) {
                        writer.Write(renameDict.Count);
                        foreach (var kvp in renameDict) {
                            writer.Write(kvp.Key);
                            writer.Write(kvp.Value);
                        }
                    }

            return renameDict;
        }

        return new std::map<std::string, std::string>();
    }

    static void buildMasterPatch(std::string esm, ILookup<std::string, std::string> knownEsmVersions) {
        var fixPath = Path.Combine(OutDir, Path.ChangeExtension(esm, ".pat"));
        if (File.Exists(fixPath))
            return;

        var ttwESM = Path.Combine(_dirTTWMain, esm);
        var ttwBytes = File.ReadAllBytes(ttwESM);
        var ttwChk = new FileValidation(ttwBytes);

        var altVersions = knownEsmVersions[esm].ToList();

        var patches =
            altVersions.Select(dataESM => {
                    var dataBytes = File.ReadAllBytes(dataESM);
                    byte[] patchBytes;

                    using (var msPatch = new MemoryStream()) {
                    MakeDiff.Create(dataBytes, ttwBytes, Diff.SIG_LZDIFF41, msPatch);
                    patchBytes = msPatch.ToArray();
                    }

                    return new PatchInfo {
                    Metadata = new FileValidation(dataESM),
                    Data = patchBytes
                    };
                    })
        .AsParallel()
            .ToArray();

        var patchDict = new PatchDict(altVersions.Count);
        patchDict.Add(esm, new Patch(ttwChk, patches));

        using (var fixStream = File.OpenWrite(fixPath))
            patchDict.WriteAll(fixStream);
    }
}
