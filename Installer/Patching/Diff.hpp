#pragma once

namespace TaleOfTwoWastelandsPatching {
    // NOTE: public class
    // NOTE: static class
    class Diff {
        public:
            const long SIG_BSDIFF40 = 0x3034464649445342;
            const long SIG_LZDIFF41 = 0x3134464649445a4c;
            const long SIG_NONONONO = 0x4f4e4f4e4f4e4f4e;
            const int HEADER_SIZE = 32;

            static Stream GetEncodingStream(Stream stream, long signature, bool output);
            static void Apply(byte* pInput, long length, byte* pPatch, long patchLength, Stream output);
            static unsafe long CreatePatchStreams(byte* pPatch, long patchLength, out Stream ctrl, out Stream diff, out Stream extra);
            static void ApplyInternal(long newSize, Stream input, Stream ctrl, Stream diff, Stream extra, Stream output);
            static unsafe long ReadInt64(byte* pb)
            static long ReadInt64(Stream ps)

        private:
            const int LZMA_DICTSIZE_ULTRA = 1024 * 1024 * 64; //64MiB, 7z 'Ultra'
            static void SetCompressionLevel()
    }
}
