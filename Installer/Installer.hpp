#pragma once

namespace TaleOfTwoWastelands {
    // NOTE: public class
    class Installer : IInstaller {
    public:
        
/* VESTIGIAL MACRO
 *  #region Statics
 */
        // NOTE: read-only block
        static std::string m_patchDir = Path.Combine(Paths.AssetsDir, "TTW Data", "TTW Patches");

/* VESTIGIAL MACRO
 *  #endregion
 */

        
/* VESTIGIAL MACRO
 *  #region Instance public properties
 */

        /// <summary>
        /// Provides progress updates for minor operations
        /// </summary>
        IProgress<InstallStatus> m_progressMinorOperation { get; set; }
        /// <summary>
        /// Provides progress updates for major operations
        /// </summary>
        IProgress<InstallStatus> m_progressMajorOperation { get; set; }

        CancellationToken m_token { get; private set; }
        
/* VESTIGIAL MACRO
 *  #endregion
 */

        Installer(ILog log, IPrompts prompts);
        void install(CancellationToken inToken);
        std::string getDirFO3Data(void);
        std::string getDirFNVData(void);
        std::string getDirTTWMain(void);
        std::string getDirTTWOptional(void);

    private:
        std::string m_dirFO3Data;
        std::string m_dirFNVData;
        std::string m_dirTTWMain;
        std::string m_dirTTWOptional;
        
/* VESTIGIAL MACRO
 *  #region Instance m_private
 */
        BsaDiff _m_bsaDiff;
        NVSE _m_nvse;

        // NOTE: read-only block
        ILog m_log;
        IPrompts m_prompts;
        
/* VESTIGIAL MACRO
 *  #endregion
 */

        std::optional<bool> handleStep<T>(IInstallStatus status);
        bool showSkipDialog(std::string description);
        void buildSFX();
        void buildVoice();
        bool patchMasters(InstallStatus opProg);
        void falloutLineCopy(std::string name, std::string path);
        static bool checkExisting(std::string path, FileValidation newChk);
        bool patchMaster(std::string esm);
        void fail(std::string msg = null);
    }
}
