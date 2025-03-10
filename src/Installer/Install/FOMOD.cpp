#include "FOMOD.hpp"

namespace TaleOfTwoWastelandsInstall {

    FOMOD::FOMOD(ILog log) : m_log(log) {
        SevenZipCompressor.SetLibraryPath(Path.Combine(Paths.AssetsDir,
                    Paths.SevenZipBinaries, Environment.Is64BitProcess
                    ? Paths.SevenZipX64 : Paths.SevenZipX32));
    }

    void buildAll(InstallStatus status, std::string mainBuildFolder, std::string optBuildFolder, std::string saveFolder) {
        Log.Dual("Building FOMODs...");
        Log.Display("This can take some time.");
        Build(status, mainBuildFolder, Path.Combine(saveFolder, Paths.MainFOMOD));
        Build(status, optBuildFolder, Path.Combine(saveFolder, Paths.OptFOMOD));
        Log.File("\tDone.");
        Log.Display("\tFOMODs built.");
    }

    static void build(InstallStatus status, std::string path, std::string fomod) {
        var compressor = new SevenZipCompressor
        {
            ArchiveFormat = OutArchiveFormat.SevenZip,
            CompressionLevel = CompressionLevel.Fast,
            CompressionMethod = CompressionMethod.Lzma2,
            CompressionMode = CompressionMode.Create,
        };
        compressor.CustomParameters.Add("mt", "on"); //enable multithreading

        compressor.FilesFound += (sender, e) => status.ItemsTotal = e.Value;
        compressor.Compressing += (sender, e) => e.Cancel = status.Token.IsCancellationRequested;
        compressor.CompressionFinished += (sender, e) => status.Finish();
        compressor.FileCompressionStarted += (sender, e) => status.CurrentOperation = "Compressing " + e.FileName;
        compressor.FileCompressionFinished += (sender, e) => status.Step();

        compressor.CompressDirectory(path, fomod, true);
    }

}
