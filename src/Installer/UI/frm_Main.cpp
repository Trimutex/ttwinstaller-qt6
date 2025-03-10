#include "frm_Main.hpp"

namespace TaleOfTwoWastelandsUI {
        frm_Main() {
            InitializeComponent();
        }

        void fromMainLoad(object sender, EventArgs e) {
            Util.AssertElevated();

            Log = DependencyRegistry.Container.GetInstance<ILog>();
            Log.DisplayMessage = new Progress<string>(UpdateLog);

			_prompts = DependencyRegistry.Container
			  .With("openDialog").EqualTo(dlg_FindGame)
			  .With("saveDialog").EqualTo(dlg_SaveTTW)
			  .GetInstance<IPrompts>();
			DependencyRegistry.Container.Inject(_prompts);

			txt_FO3Location.Text = _prompts.Fallout3Path;
			txt_FNVLocation.Text = _prompts.FalloutNVPath;
			txt_TTWLocation.Text = _prompts.TTWSavePath;

			_install = DependencyRegistry.Container.GetInstance<IInstaller>();
            _install.ProgressMajorOperation = new Progress<InstallStatus>(m => UpdateProgressBar(m, prgOverall));
            _install.ProgressMinorOperation = new Progress<InstallStatus>(m => UpdateProgressBar(m, prgCurrent));
        }

        void updateProgressBar(InstallStatus opProg, TextProgressBar bar) {
            bar.Maximum = opProg.ItemsTotal;
            bar.Value = opProg.ItemsDone;
            bar.CustomText = opProg.CurrentOperation;
        }

        void updateLog(std::string msg) {
            txt_Progress.AppendText(msg);
            txt_Progress.AppendText(Environment.NewLine);
        }

        void buttonFO3BrowseOnClick(object sender, EventArgs e) {
            _prompts.Fallout3Prompt(true);
            txt_FO3Location.Text = _prompts.Fallout3Path;
        }

        void buttonFNVBrowseOnClick(object sender, EventArgs e) {
            _prompts.FalloutNVPrompt(true);
            txt_FNVLocation.Text = _prompts.FalloutNVPath;
        }

        void buttonTTWBrowseOnClick(object sender, EventArgs e) {
            _prompts.TTWPrompt(true);
            txt_TTWLocation.Text = _prompts.TTWSavePath;
        }

        void buttonInstallOnClick(object sender, EventArgs e) {
            Action resetInstallBtn = () => {
                btn_Install.Text = Localization.Install;
                btn_Install.Enabled = true;
                _installCts.Dispose();
            };

            if (btn_Install.Text == Localization.Install) {
                _installCts = new CancellationTokenSource();

                btn_Install.Text = Localization.Cancel;
                _installTask = Task.Factory.StartNew(() => _install.Install(_installCts.Token));
                _installTask.ContinueWith(task => {
                    if (btn_Install.InvokeRequired)
                        btn_Install.Invoke(resetInstallBtn);
                    else
                        resetInstallBtn();
                });
            } else {
                btn_Install.Text = Localization.CancelingWait;
                btn_Install.Enabled = false;
                _installCts.Cancel();
            }
        }

        void checkYouCheckedChanged(object sender, EventArgs e) {
            var checkbox = (sender as CheckBox);
            Debug.Assert(checkbox != null, "checkbox != null");

            if (!checkbox.Checked) {
                checkbox.Checked = true;
                MessageBox.Show(Localization.RightSaidFred);
            }
        }
    }
}
