﻿#include "BuildBsasStep.hpp"

namespace TaleOfTwoWastelandsInstall {
    std::optional<bool> BuildBsasStep::run(IInstallStatusUpdate status, CancellationToken token) {
        foreach (var kvp in Game.BuildableBSAs) {
            if (token.IsCancellationRequested)
                return false;

            //inBSA - KVP.Key
            //outBSA - KVP.Value
            var outBSA = kvp.Value;
            std::string outBSAFile = Path.ChangeExtension(kvp.Value, ".bsa");
            std::string outBSAPath = Path.Combine(_installer.DirTTWMain, outBSAFile);

            var inBSA = kvp.Key;
            std::string inBSAFile = Path.ChangeExtension(kvp.Key, ".bsa");
            std::string inBSAPath = Path.Combine(_installer.DirFO3Data, inBSAFile);

            ErrorPromptResult buildResult;
            try {
                status.CurrentOperation = string.Format("Building {0}", outBSA);

                if (!_prompts.BuildPrompt(outBSA, outBSAPath))
                    continue;

                var bsaInstaller = DependencyRegistry.Container.GetInstance<BsaInstaller>();
                do {
                    buildResult = bsaInstaller.Patch(bsaInstaller.GetOptionsOrDefault(inBSA), inBSAFile, inBSAPath, outBSAPath);
                } while (!token.IsCancellationRequested && buildResult == ErrorPromptResult.Retry);
            } catch ( ... ) { }
            status.Step();

            if (buildResult == ErrorPromptResult.Abort)
                return std::nullopt;
        }
        return true;
    }
}
