#include "BsaInstaller.hpp"

CompressionOptions BSAInstaller::GetOptionsOrDefault(std::string inBSA) {
    CompressionOptions bsaOptions;
    if (BSAOptions.TryGetValue(inBsa, out bsaOptions))
    {
        if (bsaOptions.ExtensionCompressionLevel.Count == 0)
            bsaOptions.ExtensionCompressionLevel = DefaultBSAOptions.ExtensionCompressionLevel;
        if (bsaOptions.Strategy == CompressionStrategy.Safe)
            bsaOptions.Strategy = DefaultBSAOptions.Strategy;
    }

    return (bsaOptions) ? bsaOptions : DefaultBSAOptions;
}

ErrorPromptResult BSAInstaller::Patch(CompressionOptions options, string inBsaFile,
        string inBsaPath, string outBsaPath) {
    bool patchSuccess;

#if DEBUG
    var watch = new Stopwatch();
    try {
        watch.Start();
#endif
        patchSuccess = _bsaDiff.PatchBsa(options, inBsaPath, outBsaPath);
        if (!patchSuccess)
            Log.Dual("Patching BSA {0} failed", inBsaFile);
#if DEBUG
    } catch ( ... ) { }
    watch.Stop();
    Debug.WriteLine("PatchBSA for {0} finished in {1}", inBsaFile, watch.Elapsed);
#endif

    if (patchSuccess) {
        Log.Dual("Build successful.");
        return ErrorPromptResult.Continue;
    }

    return Prompts.PatchingErrorPrompt(inBsaFile);
}

void BSAInstaller::Extract(CancellationToken token, IEnumerable<BSAFolder> folders,
        string outBsa, string outBsaPath, bool skipExisting) {
    foreach (var folder in folders)
    {
        Directory.CreateDirectory(Path.Combine(outBsaPath, folder.Path));
        Log.File("Created " + folder.Path);

        foreach (var file in folder)
        {
            token.ThrowIfCancellationRequested();

            var filePath = Path.Combine(outBsaPath, file.Filename);
            if (File.Exists(filePath) && skipExisting)
            {
                Log.File("Skipped (already exists) " + file.Filename);
                continue;
            }

            File.WriteAllBytes(filePath, file.GetContents(true));
            Log.File("Extracted " + file.Filename);
        }
    }

    Log.File("Extract from {0} done!", outBsa);
}
