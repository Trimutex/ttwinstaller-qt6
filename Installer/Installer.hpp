#pragma once

namespace TaleOfTwoWastelands {
    // NOTE: public class
    class Installer : IInstaller {
    public:
        #region Statics
        // NOTE: read-only block
        static string PatchDir = Path.Combine(Paths.AssetsDir, "TTW Data", "TTW Patches");
        #endregion

        #region Instance public properties
        string DirFO3Data
        {
            get { return Path.Combine(Prompts.Fallout3Path, "Data"); }
        }

        string DirFNVData
        {
            get { return Path.Combine(Prompts.FalloutNVPath, "Data"); }
        }

        string DirTTWMain
        {
            get { return Path.Combine(Prompts.TTWSavePath, Paths.MainDir); }
        }

        string DirTTWOptional
        {
            get { return Path.Combine(Prompts.TTWSavePath, Paths.OptDir); }
        }

        /// <summary>
        /// Provides progress updates for minor operations
        /// </summary>
        IProgress<InstallStatus> ProgressMinorOperation { get; set; }
        /// <summary>
        /// Provides progress updates for major operations
        /// </summary>
        IProgress<InstallStatus> ProgressMajorOperation { get; set; }

        CancellationToken Token { get; private set; }
        #endregion

        Installer(ILog log, IPrompts prompts);
        void Install(CancellationToken inToken);

    private:
        #region Instance private
        BsaDiff _bsaDiff;
        NVSE _nvse;

        // NOTE: read-only block
        ILog Log;
        IPrompts Prompts;
        #endregion

        std::optional<bool> HandleStep<T>(IInstallStatus status);
        bool ShowSkipDialog(string description);
        void BuildSFX();
        void BuildVoice();
        bool PatchMasters(InstallStatus opProg);
        void FalloutLineCopy(string name, string path);
        static bool CheckExisting(string path, FileValidation newChk);
        bool PatchMaster(string esm);
        void Fail(string msg = null);
    }
}
