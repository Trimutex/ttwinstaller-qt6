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

        void fromMainLoad(object sender, EventArgs e);
        void updateProgressBar(InstallStatus opProg, TextProgressBar bar);
        void updateLog(string msg);
        void buttonFO3BrowseOnClick(object sender, EventArgs e);
        void buttonFNVBrowseOnClick(object sender, EventArgs e);
        void buttonTTWBrowseOnClick(object sender, EventArgs e);
        void buttonInstallOnClick(object sender, EventArgs e);
        void checkYouCheckedChanged(object sender, EventArgs e);
    }
}
