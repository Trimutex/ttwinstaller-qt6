#pragma once

namespace PatchMaker {
    // NOTE: internal class
    static class MakeDiff {
    public:
        static void Create(byte[] oldBuf, byte[] newBuf, long signature, Stream output);

    private:
        const int HEADER_SIZE = Diff.HEADER_SIZE;

        static long ReadInt64(byte* buf);
        static Stream GetEncodingStream(Stream stream, long signature, bool output);
        static byte[] ConvertPatch(byte* pPatch, long length, long inputSig, long outputSig);
        static int CompareBytes(byte* left, int leftLength, byte* right, int rightLength);
        static int MatchLength(byte* oldData, int oldLength, byte* newData, int newLength);
        static int Search(int* I, byte* oldData, int oldLength, byte* newData, int newLength, int newOffset, int start, int end, out int pos);
        static void WriteInt64(long y, byte* pb);
    }
}
