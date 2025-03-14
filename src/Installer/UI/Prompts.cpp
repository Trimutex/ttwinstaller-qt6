﻿#include "Prompts.hpp"

namespace TaleOfTwoWastelandsUI {
    Prompts(OpenFileDialog openDialog, SaveFileDialog saveDialog, ILog log, IEnumerable<IPathStore> store) {
        Log = log;
        Stores = store;

        this.openDialog = openDialog;
        this.saveDialog = saveDialog;

        m_fallout3Path = TryAllStoresGetPath("Fallout3");
        m_falloutNVPath = TryAllStoresGetPath("FalloutNV");
        m_ttwSavePath = TryAllStoresGetPath("TaleOfTwoWastelands");
    }

    void promptPaths() {
        Log.File("Looking for Fallout3.exe");
        if (File.Exists(Path.Combine(m_fallout3Path, "Fallout3.exe")))
            Log.File("\tFound.");
        else
            m_fallout3Path = Fallout3Prompt();

        Log.File("Looking for FalloutNV.exe");
        if (File.Exists(Path.Combine(m_falloutNVPath, "FalloutNV.exe")))
            Log.File("\tFound.");
        else
            m_falloutNVPath = FalloutNVPrompt();

        Log.File("Looking for Tale of Two Wastelands");
        if (m_ttwSavePath != null && TTWSavePath != "\\")
            Log.File("\tDefault path found.");
        else
            m_ttwSavePath = TTWPrompt();
    }

    std::string getFO3Path(void) {
        return m_fallout3Path;
    }

    std::string getFNVPath(void) {
        return m_falloutNVPath;
    }

    std::string getTTWSavePath(void) {
        return m_ttwSavePath;
    }

    std::string fallout3Prompt(bool manual = false) {
        openDialog.FilterIndex = 1;
        openDialog.Title = Localization.Fallout3;
        return (m_fallout3Path = FindByUserPrompt(openDialog, Localization.Fallout3, "Fallout3", manual));
    }

    std::string falloutNVPrompt(bool manual = false) {
        openDialog.FilterIndex = 2;
        openDialog.Title = Localization.FalloutNewVegas;
        return (m_falloutNVPath = FindByUserPrompt(openDialog, Localization.FalloutNewVegas, "FalloutNV", manual));
    }

    std::string ttwPrompt(bool manual = false) {
        return (m_ttwSavePath = FindByUserPrompt(saveDialog, "Tale of Two Wastelands", "TaleOfTwoWastelands", manual));
    }

    bool overwritePrompt(std::string name, std::string path) {
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

    bool buildPrompt(std::string name, std::string path) {
        if (!OverwritePrompt(name, path))
            return false;

        Log.Dual("Building {0}", name);
        return true;
    }

    bool buildFOMODsPrompt() {
        return
            MessageBox.Show(
                    string.Format(Localization.BuildFOMODsPrompt, Localization.TTW, Localization.SuggestedModManager),
                    Localization.BuildFOMODsQuestion, MessageBoxButtons.YesNo) == DialogResult.Yes;
    }

    ErrorPromptResult patchingErrorPrompt(std::string file) {
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

    std::string tryAllStoresGetPath(std::string key) {
        return (from store in Stores
                let val = store.GetPathFromKey(key)
                select val)
            .FirstOrDefault(v => !string.IsNullOrEmpty(v));
    }

    bool tryAllStoresSetPath(std::string key, std::string path) {
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

    std::string findByUserPrompt(FileDialog dialog, std::string name, std::string keyName, bool manual = false) {
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
