#include "Prompts.hpp"

namespace TaleOfTwoWastelandsUI {
    Prompts(OpenFileDialog openDialog, SaveFileDialog saveDialog, ILog log, IEnumerable<IPathStore> store) {
        Log = log;
        Stores = store;

        this.openDialog = openDialog;
        this.saveDialog = saveDialog;

        Fallout3Path = TryAllStoresGetPath("Fallout3");
        FalloutNVPath = TryAllStoresGetPath("FalloutNV");
        TTWSavePath = TryAllStoresGetPath("TaleOfTwoWastelands");
    }

    void PromptPaths() {
        Log.File("Looking for Fallout3.exe");
        if (File.Exists(Path.Combine(Fallout3Path, "Fallout3.exe")))
            Log.File("\tFound.");
        else
            Fallout3Path = Fallout3Prompt();

        Log.File("Looking for FalloutNV.exe");
        if (File.Exists(Path.Combine(FalloutNVPath, "FalloutNV.exe")))
            Log.File("\tFound.");
        else
            FalloutNVPath = FalloutNVPrompt();

        Log.File("Looking for Tale of Two Wastelands");
        if (TTWSavePath != null && TTWSavePath != "\\")
            Log.File("\tDefault path found.");
        else
            TTWSavePath = TTWPrompt();
    }

    string Fallout3Prompt(bool manual = false) {
        openDialog.FilterIndex = 1;
        openDialog.Title = Localization.Fallout3;
        return (Fallout3Path = FindByUserPrompt(openDialog, Localization.Fallout3, "Fallout3", manual));
    }

    string FalloutNVPrompt(bool manual = false) {
        openDialog.FilterIndex = 2;
        openDialog.Title = Localization.FalloutNewVegas;
        return (FalloutNVPath = FindByUserPrompt(openDialog, Localization.FalloutNewVegas, "FalloutNV", manual));
    }

    string TTWPrompt(bool manual = false) {
        return (TTWSavePath = FindByUserPrompt(saveDialog, "Tale of Two Wastelands", "TaleOfTwoWastelands", manual));
    }

    bool OverwritePrompt(string name, string path) {
        if (!File.Exists(path)) {
            Log.File(Localization.FileDoesNotExist, path);
            return true;
        }

        Log.File(Localization.FileAlreadyExists, path);

        var promptResult = MessageBox.Show(string.Format(Localization.RebuildPrompt, name), Localization.FileAlreadyExistsTitle, MessageBoxButtons.YesNo);
        switch (promptResult) {
            case DialogResult.Yes:
                File.Delete(path);
                Log.File("Rebuilding {0}", name);
                return true;
            case DialogResult.No:
                Log.Dual("{0} has already been built. Skipping.", name);
                break;
        }

        return false;
    }

    bool BuildPrompt(string name, string path) {
        if (!OverwritePrompt(name, path))
            return false;

        Log.Dual("Building {0}", name);
        return true;
    }

    bool BuildFOMODsPrompt() {
        return
            MessageBox.Show(
                    string.Format(Localization.BuildFOMODsPrompt, Localization.TTW, Localization.SuggestedModManager),
                    Localization.BuildFOMODsQuestion, MessageBoxButtons.YesNo) == DialogResult.Yes;
    }

    ErrorPromptResult PatchingErrorPrompt(string file) {
        var promptResult = MessageBox.Show(string.Format(Localization.ErrorWhilePatching, file), Localization.Error, MessageBoxButtons.AbortRetryIgnore);
        switch (promptResult) {
            //case DialogResult.Abort: //Quit install
            case DialogResult.Retry:   //Start over from scratch
                Log.Dual("Retrying build.");
                return ErrorPromptResult.Retry;
            case DialogResult.Ignore:  //Ignore errors and move on
                Log.Dual("Ignoring errors.");
                return ErrorPromptResult.Continue;
        }

        return ErrorPromptResult.Abort;
    }

    string TryAllStoresGetPath(string key) {
        return (from store in Stores
                let val = store.GetPathFromKey(key)
                select val)
            .FirstOrDefault(v => !string.IsNullOrEmpty(v));
    }

    bool TryAllStoresSetPath(string key, string path) {
        foreach (var store in Stores) {
            try {
                store.SetPathFromKey(key, path);
                return true;
            } catch (InvalidOperationException) {
                continue;
            }
        }

        return false;
    }

    string FindByUserPrompt(FileDialog dialog, string name, string keyName, bool manual = false) {
        Log.File("Prompting user for {0}'s path.", name);
        MessageBox.Show(string.Format("Please select {0}'s location.", name));

        var dlgResult = dialog.ShowDialog();
        if (dlgResult == DialogResult.OK) {
            var path = Path.GetDirectoryName(dialog.FileName);
            Log.File("User {2}changed {0} directory to '{1}'", name, path, manual ? "manually " : " ");

            if (!TryAllStoresSetPath(keyName, path))
                Log.Dual(Localization.FailedToSavePath, path, keyName);

            return path;
        }

        return null;
    }
}
