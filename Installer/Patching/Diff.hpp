#pragma once

#include <sstream>
#include <string>

namespace TaleOfTwoWastelandsPatching {
    // NOTE: public class
    // NOTE: static class
    class Diff {
        public:
            const long SIGBSDIFF40 = 0x3034464649445342;
            const long SIGLZDIFF41 = 0x3134464649445a4c;
            const long SIGNONONONO = 0x4f4e4f4e4f4e4f4e;
            const int HEADERSIZE = 32;

            static std::ostringstream getEncodingStream(std::ostringstream stream, long signature, bool output);
            static void apply(uint8_t* pInput, long length, uint8_t* pPatch, long patchLength, std::ostringstream output);
            static long createPatchStreams(uint8_t* pPatch, long patchLength, out std::ostringstream ctrl, out std::ostringstream diff, out std::ostringstream extra);
            static void applyInternal(long newSize, std::ostringstream input, std::ostringstream ctrl, std::ostringstream diff, std::ostringstream extra, std::ostringstream output);
            static long readInt64(uint8_t* pb);
            static long readInt64(std::ostringstream ps);

        private:
            const int LZMADICTSIZEULTRA = 1024 * 1024 * 64; //64MiB, 7z 'Ultra'
            static void setCompressionLevel();
    }
}
