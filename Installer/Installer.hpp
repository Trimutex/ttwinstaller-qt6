#pragma once

namespace TaleOfTwoWastelands {
    // NOTE: public class
    class Installer : IInstaller {
    public:
        #region Statics
        // NOTE: read-only block
        static string m_patchDir = Path.Combine(Paths.AssetsDir, "TTW Data", "TTW Patches");
        #endregion

        #region Instance public properties
        string m_dirFO3Data
        {
            get { return Path.Combine(Prompts.Fallout3Path, "Data"); }
        }

        string m_dirFNVData
        {
            get { return Path.Combine(Prompts.FalloutNVPath, "Data"); }
        }

        string m_dirTTWMain
        {
            get { return Path.Combine(Prompts.TTWSavePath, Paths.MainDir); }
        }

        string m_dirTTWOptional
        {
            get { return Path.Combine(Prompts.TTWSavePath, Paths.OptDir); }
        }

        /// <summary>
        /// Provides progress updates for minor operations
        /// </summary>
        IProgress<InstallStatus> m_progressMinorOperation { get; set; }
        /// <summary>
        /// Provides progress updates for major operations
        /// </summary>
        IProgress<InstallStatus> m_progressMajorOperation { get; set; }

        CancellationToken m_token { get; private set; }
        #endregion

        Installer(ILog log, IPrompts prompts);
        void install(CancellationToken inToken);

    private:
        #region Instance m_private
        BsaDiff _m_bsaDiff;
        NVSE _m_nvse;

        // NOTE: read-only block
        ILog m_log;
        IPrompts m_prompts;
        #endregion

        std::optional<bool> handleStep<T>(IInstallStatus status);
        bool showSkipDialog(string description);
        void buildSFX();
        void buildVoice();
        bool patchMasters(InstallStatus opProg);
        void falloutLineCopy(string name, string path);
        static bool checkExisting(string path, FileValidation newChk);
        bool patchMaster(string esm);
        void fail(string msg = null);
    }
}
