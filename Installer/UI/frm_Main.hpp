#pragma once

namespace TaleOfTwoWastelandsUI {
    // NOTE: public class
    // NOTE: partial class
	class frm_Main : Form {
    public:
        frm_Main();

    private:
        CancellationTokenSource _installCts;
        Task _installTask;

        ILog Log;
        IInstaller _install;
        IPrompts _prompts;

        void frm_Main_Load(object sender, EventArgs e);
        void UpdateProgressBar(InstallStatus opProg, TextProgressBar bar);
        void UpdateLog(string msg);
        void btn_FO3Browse_Click(object sender, EventArgs e);
        void btn_FNVBrowse_Click(object sender, EventArgs e);
        void btn_TTWBrowse_Click(object sender, EventArgs e);
        void btn_Install_Click(object sender, EventArgs e);
        void chkYou_CheckedChanged(object sender, EventArgs e);
    }
}
