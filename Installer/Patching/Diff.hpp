#pragma once

namespace TaleOfTwoWastelandsPatching {
    // NOTE: public class
    // NOTE: static class
    class Diff {
        public:
            const long SIGBSDIFF40 = 0x3034464649445342;
            const long SIGLZDIFF41 = 0x3134464649445a4c;
            const long SIGNONONONO = 0x4f4e4f4e4f4e4f4e;
            const int HEADERSIZE = 32;

            static Stream getEncodingStream(Stream stream, long signature, bool output);
            static void apply(byte* pInput, long length, byte* pPatch, long patchLength, Stream output);
            static unsafe long createPatchStreams(byte* pPatch, long patchLength, out Stream ctrl, out Stream diff, out Stream extra);
            static void applyInternal(long newSize, Stream input, Stream ctrl, Stream diff, Stream extra, Stream output);
            static unsafe long readInt64(byte* pb);
            static long readInt64(Stream ps);

        private:
            const int LZMADICTSIZEULTRA = 1024 * 1024 * 64; //64MiB, 7z 'Ultra'
            static void setCompressionLevel();
    }
}
