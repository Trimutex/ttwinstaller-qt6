#pragma once

#include <map>
#include <string>

namespace TaleOfTwoWastelandsInstaller {
    class BsaInstaller {
        // TODO: BSAsharp replacements needed here
        const CompressionStrategy m_fastStrategy = CompressionStrategy.Unsafe | CompressionStrategy.Speed;
        const CompressionStrategy m_goodStrategy = CompressionStrategy.Unsafe | CompressionStrategy.Size;

        // TODO: These are readonly variables, prob needs to be kept that way
        // TODO: Probably BSAsharp replacement here too for options
        static std::map<std::string, CompressionOptions> m_bsaOptions;
        static CompressionOptions m_defaultBSAOptions = new CompressionOptions
        {
            Strategy = m_goodStrategy;
            std::map<std::string, int> ExtensionCompressionLevel =
            {
                {".ogg", -1},
                {".wav", -1},
                {".mp3", -1},
                {".lip", -1}
            };
        };

        BsaInstaller(ILog log, IPrompts prompts, IBSADiff bsaDiff) :
            m_log(log), m_prompts(prompts), _m_bsaDiff(bsaDiff) {};

        CompressionOptions GetOptionsOrDefault(std::string inBSA);
        ErrorPromptResult Patch(CompressionOptions options, string inBsaFile, string inBsaPath, string outBsaPath);
        void Extract(CancellationToken token, IEnumerable<BSAFolder> folders, string outBsa, string outBsaPath, bool skipExisting);


    private:
        ILog m_log;
        IPrompts m_prompts;
        IBsaDiff _m_bsaDiff;
    };
}

