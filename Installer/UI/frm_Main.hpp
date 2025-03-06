#pragma once

namespace TaleOfTwoWastelandsUI {
    // NOTE: public class
    // NOTE: partial class
	class frm_Main : Form {
    public:
        frm_Main();

    private:
        CancellationTokenSource _m_installCts;
        Task _m_installTask;

        ILog m_log;
        IInstaller _m_install;
        IPrompts _m_prompts;

        void frm_Main_Load(object sender, EventArgs e);
        void updateProgressBar(InstallStatus opProg, TextProgressBar bar);
        void updateLog(string msg);
        void btn_FO3Browse_Click(object sender, EventArgs e);
        void btn_FNVBrowse_Click(object sender, EventArgs e);
        void btn_TTWBrowse_Click(object sender, EventArgs e);
        void btn_Install_Click(object sender, EventArgs e);
        void chkYou_CheckedChanged(object sender, EventArgs e);
    }
}
