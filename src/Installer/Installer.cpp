﻿#include "Installer.hpp"

namespace TaleOfTwoWastelands {

        Installer(ILog log, IPrompts prompts) {
            Log = log;
            Prompts = prompts;

            Log.File("Version {0}", Application.ProductVersion);
            Log.File("{0}-bit architecture found.", Environment.Is64BitOperatingSystem ? "64" : "32");
        }

        void install(CancellationToken inToken) {
            var LinkedSource = CancellationTokenSource.CreateLinkedTokenSource(inToken);

            Token = LinkedSource.Token;

            Prompts.PromptPaths();

            _bsaDiff = DependencyRegistry.Container
                //.With("progress").EqualTo(ProgressMinorOperation)
                //.With("token").EqualTo(Token)
                .GetInstance<BsaDiff>();
            _nvse = DependencyRegistry.Container
                .With("FNVPath").EqualTo(Prompts.FalloutNVPath)
                .GetInstance<NVSE>();

            var opProg = new InstallStatus(ProgressMajorOperation, Token) { ItemsTotal = 7 + Game.BuildableBSAs.Count + Game.CheckedESMs.Length };
            try {
                HandleStep<CheckingRequiredFilesStep>(opProg);

                if (!_nvse.Check()) {
                    string err = null;
                    //true : should download, continue install
                    //false: should not download, continue install
                    //null : should not download, abort install
                    switch (_nvse.Prompt()) {
                        case true:
                            if (_nvse.Install(out err))
                                break;
                            goto default;
                        case false:
                            break;
                        default:
                            Fail(err);
                            return;
                    }
                }

                try {
                    const string curOp = "Creating FOMOD foundation";
                    opProg.CurrentOperation = curOp;

                    Log.File(curOp);

                    string
                        srcFolder = Path.Combine(Paths.AssetsDir, "TTW Data", "TTW Files"),
                        tarFolder = Prompts.TTWSavePath;

                    Util.CopyFolder(srcFolder, tarFolder);
                } catch ( ... ) { }
                opProg.Step();

                //count BuildableBSAs
                HandleStep<BuildBsasStep>(opProg);

                try
                {
                    opProg.CurrentOperation = "Building SFX";

                    BuildSFX();
                } catch ( ... ) { }
                opProg.Step();

                try
                {
                    opProg.CurrentOperation = "Building Voices";

                    BuildVoice();
                } catch ( ... ) { }
                opProg.Step();

                try
                {
                    const string ttwArchive = "TaleOfTwoWastelands.bsa";
                    opProg.CurrentOperation = "Copying " + ttwArchive;

                    if (!File.Exists(Path.Combine(DirTTWMain, ttwArchive)))
                        File.Copy(Path.Combine(Paths.AssetsDir, "TTW Data", ttwArchive), Path.Combine(DirTTWMain, ttwArchive));
                } catch ( ... ) { }
                opProg.Step();

                //count CheckedESMs
                if (!PatchMasters(opProg))
                    return;

                //+2
                {
                    const string prefix = "Copying ";
                    const string opA = "Fallout3 music files";
                    const string opB = "Fallout3 video files";

                    opProg.CurrentOperation = prefix + opA;
                    FalloutLineCopy(opA, Path.Combine(Paths.AssetsDir, "TTW Data", "FO3_MusicCopy.txt"));
                    opProg.Step();

                    opProg.CurrentOperation = prefix + opB;
                    FalloutLineCopy(opB, Path.Combine(Paths.AssetsDir, "TTW Data", "FO3_VideoCopy.txt"));
                    opProg.Step();
                }

                HandleStep<BuildFOMODsStep>(opProg);

                opProg.Finish();

                Log.Display("Install completed successfully.");
                MessageBox.Show(string.Format(Localization.InstalledSuccessfully, Localization.TTW));
            } catch (OperationCanceledException) {
                //intentionally cancelled - swallow exception
                Log.Dual("Install was cancelled.");
            } catch (Exception ex) {
                Log.File(ex.ToString());
                Fail("An error interrupted the install!");
                MessageBox.Show(string.Format(Localization.ErrorWhileInstalling, ex.Message), Localization.Error);
            }
        }

        std::string getDirFO3Data(void) {
            return Path.Combine(Prompts.Fallout3Path, "Data");
        }

        std::string getDirFNVData(void) {
            return Path.Combine(Prompts.FalloutNVPath, "Data");
        }

        std::string getDirTTWMain(void) {
            return Path.Combine(Prompts.TTWSavePath, Paths.MainDir);
        }

        std::string getDirTTWOptional(void) {
            return Path.Combine(Prompts.TTWSavePath, Paths.OptDir);
        }

        std::optional<bool> handleStep<T>(IInstallStatus status) where T: IInstallStep {
            var step = DependencyRegistry.Container.GetInstance<T>();
            try {
                return step.Run(status, Token);
            } catch (std::exception& e) {
                Log.File("Exception on install step {0}", typeof(T));
                throw;
            } catch ( ... ) { }
            status.Step();   
        }

        bool showSkipDialog(std::string description) {
            switch (MessageBox.Show(string.Format(Localization.AlreadyExistOverwritePrompt, description), Localization.OverwriteFiles, MessageBoxButtons.YesNo)) {
                case DialogResult.Yes:
                    return false;
                default:
                    return true;
            }
        }

        void buildSFX() {
            var fo3BsaPath = Path.Combine(DirFO3Data, "Fallout - Sound.bsa");

            var songsPath = Path.Combine("sound", "songs");

            bool skipSongs = false, skipSFX = false;
            if (Directory.Exists(Path.Combine(DirTTWMain, songsPath)))
                skipSongs = ShowSkipDialog("Fallout 3 songs");

            var outBsaPath = Path.Combine(DirTTWOptional, "Fallout3 Sound Effects", "TaleOfTwoWastelands - SFX.bsa");
            if (File.Exists(outBsaPath))
                skipSFX = ShowSkipDialog("Fallout 3 sound effects");

            if (skipSongs && skipSFX)
                return;

            var bsaInstaller = DependencyRegistry.Container.GetInstance<BsaInstaller>();
            using (BSA
               inBsa = new BSA(fo3BsaPath),
               outBsa = new BSA(inBsa.Settings)) {
                if (!skipSongs) {
                    Log.Display("Extracting songs");
                    bsaInstaller.Extract(Token, inBsa.Where(folder => folder.Path.StartsWith(songsPath)), "Fallout - Sound", DirTTWMain, false);
                }

                if (skipSFX)
                    return;

                Log.Display("Building optional TaleOfTwoWastelands - SFX.bsa...");

                var fxuiPath = Path.Combine("sound", "fx", "ui");

                var includedFilenames = new HashSet<string>(File.ReadLines(Path.Combine(Paths.AssetsDir, "TTW Data", "TTW_SFXCopy.txt")));

                var includedGroups =
                    from folder in inBsa.Where(folder => folder.Path.StartsWith(fxuiPath))
                    from file in folder
                    where includedFilenames.Contains(file.Filename)
                    group file by folder;

                foreach (var group in includedGroups) {
                    //make folder only include files that matched includedFilenames
                    @group.Key.IntersectWith(@group);

                    //add folders back into output BSA
                    outBsa.Add(@group.Key);
                }

                Log.File("Building TaleOfTwoWastelands - SFX.bsa.");
                outBsa.Save(outBsaPath);

                Log.Display("\tDone");
            }
        }

        void buildVoice() {
            var outBsaPath = Path.Combine(DirTTWOptional, "Fallout3 Player Voice", "TaleOfTwoWastelands - PlayerVoice.bsa");
            if (File.Exists(outBsaPath))
                return;

            var inBsaPath = Path.Combine(DirFO3Data, "Fallout - Voices.bsa");

            using (BSA
                inBsa = new BSA(inBsaPath),
                outBsa = new BSA(inBsa.Settings)) {
                var includedFolders = inBsa
                    .Where(folder => Game.VoicePaths.ContainsKey(folder.Path))
                    .Select(folder => new BSAFolder(Game.VoicePaths[folder.Path], folder));

                foreach (var folder in includedFolders)
                    outBsa.Add(folder);

                outBsa.Save(outBsaPath);
            }
        }

        bool PatchMasters(InstallStatus opProg) {
            foreach (var esm in Game.CheckedESMs)
                try {
                    opProg.CurrentOperation = "Patching " + esm;

                    if (Token.IsCancellationRequested || !PatchMaster(esm))
                        return false;
                } catch ( ... ) { }
                opProg.Step();

            return true;
        }

        void FalloutLineCopy(std::string name, std::string path) {
            bool skipExisting = false, asked = false;

            Log.Dual("Copying " + name + "...");
            foreach (var line in File.ReadLines(path)) {
                var ttwLinePath = Path.Combine(DirTTWMain, line);
                var foLinePath = Path.Combine(DirFO3Data, line);

                var newDirectory = Path.GetDirectoryName(ttwLinePath);
                Directory.CreateDirectory(newDirectory);

                if (File.Exists(foLinePath)) {
                    if (File.Exists(ttwLinePath) && !asked) {
                        if (skipExisting)
                            continue;
                        else {
                            asked = true;
                            skipExisting = ShowSkipDialog(name);
                        }
                    }

                    try {
                        File.Copy(foLinePath, ttwLinePath, true);
                    } catch (UnauthorizedAccessException error) {
                        Log.File("\tERROR: " + line + " did not copy successfully due to: Unauthorized Access Exception " + error.Source + ".");
                    }
                } else
                    Log.File("\tFile not found:\t" + foLinePath);
            }
            Log.Dual("\tDone.");
        }

        static bool CheckExisting(std::string path, FileValidation newChk) {
            using (var existingChk = new FileValidation(path, newChk.Type))
                return newChk == existingChk;
        }

        bool PatchMaster(std::string esm) {
            Log.Dual("Patching " + esm + "...");

            var patchPath = Path.Combine(PatchDir, Path.ChangeExtension(esm, ".pat"));
            if (File.Exists(patchPath)) {
                var patchDict = new PatchDict(patchPath);

                Debug.Assert(patchDict.ContainsKey(esm));
                var patch = patchDict[esm];
                var patches = patch.Item2;
                var newChk = patch.Item1;

                var finalPath = Path.Combine(DirTTWMain, esm);
                if (File.Exists(finalPath)) {
                    Log.Dual("\t" + esm + " already exists");
                    if (CheckExisting(finalPath, newChk)) {
                        Log.Dual("\t" + esm + " is up to date");
                        return true;
                    }

                    Log.Dual("\t" + esm + " is out of date");
                }

                var dataPath = Path.Combine(DirFO3Data, esm);
                //TODO: change to a user-friendly condition and message
                Trace.Assert(File.Exists(dataPath));

                //make sure we didn't include old patches by mistake
                Debug.Assert(patches.All(p => p.Metadata.Type == FileValidation.ChecksumType.Murmur128));

                using (var dataChk = new FileValidation(dataPath)) {
                    var matchPatch = patches.SingleOrDefault(p => p.Metadata == dataChk);
                    if (matchPatch == null) {
                        Log.Display("\tA patch for your version of " + esm + " could not be found");
                        Log.File("\tA patch for " + esm + " version " + dataChk + " could not be found");
                    } else {
                        using (FileStream
                            dataStream = File.OpenRead(dataPath),
                            outputStream = File.Open(finalPath, FileMode.Create, FileAccess.ReadWrite)) {
                            FileValidation outputChk;
                            if (matchPatch.PatchStream(dataStream, newChk, outputStream, out outputChk)) {
                                Log.Dual("\tPatch successful");
                                return true;
                            }

                            Log.File("\tPatch failed - " + outputChk);
                        }
                    }
                }
            }
            else
                Log.Dual("\t" + esm + " patch is missing from " + PatchDir);

            Fail("Your version of " + esm + " cannot be patched. This is abnormal.");

            return false;
        }

        void Fail(std::string msg = null) {
            if (msg != null)
                Log.Dual(msg);
            Log.Dual("Install aborted.");
        }
}
