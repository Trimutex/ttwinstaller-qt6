﻿#include "BSADiff.hpp"

namespace TaleOfTwoWastelandsPatching {

    bool patchBsa(CompressionOptions bsaOptions, std::string oldBSA, std::string newBSA, bool simulate = false) {
        var Op = new InstallStatus(Progress, Token) { ItemsTotal = 7 };

        var outBsaFilename = Path.GetFileNameWithoutExtension(newBSA);

        BSA bsa;
        try
        {
            Op.CurrentOperation = "Opening " + Path.GetFileName(oldBSA);

            bsa = new BSA(oldBSA, bsaOptions);
        }
        finally
        {
            Op.Step();
        }

        IDictionary<string, string> renameDict;
        try
        {
            Op.CurrentOperation = "Opening rename database";


/* VESTIGIAL MACRO
 *  #if LEGACY
 */
            var renamePath = Path.Combine(Installer.PatchDir, outBsaFilename, "RenameFiles.dict");

/* VESTIGIAL MACRO
 *  #else
 */
            var renamePath = Path.Combine(Installer.PatchDir, Path.ChangeExtension(outBsaFilename, ".ren"));

/* VESTIGIAL MACRO
 *  #endif
 */
            if (File.Exists(renamePath))
            {

/* VESTIGIAL MACRO
 *  #if LEGACY
 */
                renameDict = new Dictionary<string, string>(Util.ReadOldDatabase(renamePath));

/* VESTIGIAL MACRO
 *  #else
 */
                using (var fileStream = File.OpenRead(renamePath))
                    using (var lzmaStream = new LzmaDecodeStream(fileStream))
                    using (var reader = new BinaryReader(lzmaStream))
                    {
                        var numPairs = reader.ReadInt32();
                        renameDict = new Dictionary<string, string>(numPairs);

                        while (numPairs-- > 0)
                            renameDict.Add(reader.ReadString(), reader.ReadString());
                    }

/* VESTIGIAL MACRO
 *  #endif
 */
            }
            else
                renameDict = new Dictionary<string, string>();
        }
        finally
        {
            Op.Step();
        }

        PatchDict patchDict;
        try
        {
            Op.CurrentOperation = "Opening patch database";


/* VESTIGIAL MACRO
 *  #if LEGACY
 */
            var chkPrefix = Path.Combine(Installer.PatchDir, outBsaFilename);
            var chkPath = Path.Combine(chkPrefix, "CheckSums.dict");
            patchDict = PatchDict.FromOldDatabase(Util.ReadOldDatabase(chkPath), chkPrefix, b => b);

/* VESTIGIAL MACRO
 *  #else
 */
            var patchPath = Path.Combine(Installer.PatchDir, Path.ChangeExtension(outBsaFilename, ".pat"));
            if (File.Exists(patchPath))
                patchDict = new PatchDict(patchPath);
            else {
                m_log.Dual("\tNo patch database is available for: " + oldBSA);
                return false;
            }

/* VESTIGIAL MACRO
 *  #endif
 */
        } catch ( ... ) { }
        Op.step();

        using (bsa) {
            try {
                RenameFiles(bsa, renameDict);

                if (renameDict.Count > 0) {
                    foreach (var kvp in renameDict) {
                        m_log.Dual("File not found: " + kvp.Value);
                        m_log.Dual("\tCannot create: " + kvp.Key);
                    }
                }
            } catch ( ... ) { }
            Op.step();

            var allFiles = bsa.SelectMany(folder => folder).ToList();
            try {
                var opChk = new InstallStatus(Progress, Token) { ItemsTotal = patchDict.Count };

                var joinedPatches = from patKvp in patchDict
                    //if the join is not grouped, this will exclude missing files, and we can't find and fail on them
                    join oldFile in allFiles on patKvp.Key equals oldFile.Filename into foundOld
                    join bsaFile in allFiles on patKvp.Key equals bsaFile.Filename
                    select new PatchJoin(bsaFile, foundOld.SingleOrDefault(), patKvp.Value);

/* VESTIGIAL MACRO
 *  #if DEBUG
 */
                var watch = new Stopwatch();
                try {
                    watch.Start();

/* VESTIGIAL MACRO
 *  #endif
 *  #if PARALLEL  
 */
                    Parallel.ForEach(joinedPatches, join =>

/* VESTIGIAL MACRO
 *  #else
 */
                            foreach (var join in joinedPatches)

/* VESTIGIAL MACRO
 *  #endif
 */
                            HandleFile(opChk, join)

/* VESTIGIAL MACRO
 *  #if PARALLEL
 */
                            )

/* VESTIGIAL MACRO
 *  #endif
 */
                        ;

/* VESTIGIAL MACRO
 *  #if DEBUG
 */
                } catch ( ... ) { }
                watch.stop();
                Debug.WriteLine(outBsaFilename + " HandleFile loop finished in " + watch.Elapsed);

/* VESTIGIAL MACRO
 *  #endif
 */
            } catch ( ... ) { }
            Op.step();

            try {
                Op.CurrentOperation = "Removing unnecessary files";

                var notIncluded = allFiles.Where(file => !patchDict.ContainsKey(file.Filename));
                var filesToRemove = new HashSet<BSAFile>(notIncluded);

                foreach (BSAFolder folder in bsa)
                    folder.RemoveWhere(filesToRemove.Contains);

                var emptyFolders = bsa.Where(folder => folder.Count == 0).ToList();
                emptyFolders.ForEach(folder => bsa.Remove(folder));
            } catch ( ... ) { }
            Op.step();

            try {
                Op.CurrentOperation = "Saving " + Path.GetFileName(newBSA);

                if (!simulate)
                    bsa.Save(newBSA.ToLowerInvariant());
            } catch ( ... ) { }
            Op.step();
        }

        Op.finish();

        return true;
    }

    static IEnumerable<Tuple<std::string, std::string, std::string>> createRenameQuery(BSA bsa, std::map<std::string, std::string> renameDict) {
        //TODO: use dict union
        var renameGroup = from folder in bsa
            from file in folder
            join kvp in renameDict on file.Filename equals kvp.Value
            let a = new { folder, file, kvp }
        //group a by kvp.Value into g
        select a;

        var renameCopies = from g in renameGroup
            let newFilename = g.kvp.Key
            let newDirectory = Path.GetDirectoryName(newFilename)
            let a = new { g.folder, g.file, newFilename }
        group a by newDirectory into outs
            select outs;

        var newBsaFolders = renameCopies.ToList();
        newBsaFolders.ForEach(g => bsa.Add(new BSAFolder(g.Key)));

        return from g in newBsaFolders
            from a in g
            join folder in bsa on g.Key equals folder.Path
            let newFile = a.file.DeepCopy(g.Key, Path.GetFileName(a.newFilename))
            let addedFile = folder.Add(newFile)
            select Tuple.Create(a.file.Name, newFile.Name, a.newFilename);
    }

    bool patchBsaFile(BSAFile bsaFile, PatchInfo patch, FileValidation targetChk) {
        //InflaterInputStream won't let the patcher seek it,
        //so we have to perform a new allocate-and-copy
        uint8_t[]
            inputBytes = bsaFile.getContents(true),
                       outputBytes;

        FileValidation outputChk;

        var success = patch.patchBytes(inputBytes, targetChk, out outputBytes, out outputChk);
        using (outputChk) {
            if (success) {
                bsaFile.updateData(outputBytes, false);
                return true;
            }

            m_log.File("ERROR: Patching " + bsaFile.Filename + " has failed - " + outputChk);
            return false;
        }
    }

    void renameFiles(BSA bsa, std::map<std::string, std::string> renameDict) {
        const string opPrefix = "Renaming BSA files";

        var opRename = new InstallStatus(Progress, Token) { CurrentOperation = opPrefix };

        var renameFixes = CreateRenameQuery(bsa, renameDict);
        opRename.ItemsTotal = renameDict.Count;


/* VESTIGIAL MACRO
 *  #if PARALLEL
 */
        Parallel.ForEach(renameFixes, a =>

/* VESTIGIAL MACRO
 *  #else
 */
                foreach (var a in renameFixes)

/* VESTIGIAL MACRO
 *  #endif
 */
                {
                renameDict.Remove(a.Item3);

                opRename.CurrentOperation = opPrefix + ": " + a.Item1 + " -> " + a.Item2;
                opRename.Step();
                }

/* VESTIGIAL MACRO
 *  #if PARALLEL
 */
                )

/* VESTIGIAL MACRO
 *  #endif
 */
            ;
    }

    IProgress<InstallStatus> getProgress(void) {
        return _m_installer.ProgressMinorOperation;
    }

    CancellationToken getToken(void) {
        return _m_installer.Token;
    }

    void handleFile(InstallStatus opChk, PatchJoin join) {
        try {
            var newFile = join.Item1;
            var oldFile = join.Item2;

            var filepath = newFile.Filename;
            var filename = newFile.Name;

            if (oldFile == null) {
                m_log.Dual("ERROR: File not found: " + filepath);
                return;
            }

            var patchTuple = join.Item3;
            var newChk = patchTuple.Item1;
            var patches = patchTuple.Item2;

            if (filepath.StartsWith(Game.VoicePrefix) && (patches == null || patches.Length == 0)) {
                opChk.CurrentOperation = "Skipping " + filename;
                //m_log.File("Skipping voice file: " + filepath);
                return;
            }

            using (var curChk = FileValidation.FromBSAFile(oldFile, newChk.Type))
                if (newChk == curChk) {
                    opChk.CurrentOperation = "Compressing " + filename;
                    newFile.Cache();
                } else {
                    //YOUR HANDY GUIDEBOOK FOR STRANGE CHECKSUM ACRONYMS!
                    //newChk - the checksum for the expected final result (after patching)
                    //oldChk - the checksum for the original file a diff is built against
                    //curChk - the checksum for the current file being compared or patched
                    //tstChk - the checksum for the current file, in the format of oldChk
                    //patChk - the checksum for the current file, after patching or failure
                    foreach (var patchInfo in patches) {
                        var oldChk = patchInfo.Metadata;

                        if (curChk.Type != oldChk.Type) {
                            using (var tstChk = FileValidation.FromBSAFile(oldFile, oldChk.Type))
                                if (oldChk != tstChk)
                                    //this is a patch for a different original
                                    continue;
                        } else if (oldChk != curChk)
                            //this is a patch for a different original
                            continue;

                        //patch is for this original
                        opChk.CurrentOperation = "Patching " + filename;

                        if (PatchBsaFile(newFile, patchInfo, newChk))
                            return;
                        else
                            m_log.Dual("ERROR: Patching " + filepath + " failed");
                    }

                    using (var patChk = FileValidation.FromBSAFile(newFile, newChk.Type))
                        if (newChk != patChk) {
                            //no patch exists for the file
                            m_log.Dual("WARNING: File is of an unexpected version: " + newFile.Filename + " - " + patChk);
                            m_log.Dual("This file cannot be patched. Errors may occur.");
                        }
                }
        } catch ( ... ) { }
        opChk.Step();
    }

}
