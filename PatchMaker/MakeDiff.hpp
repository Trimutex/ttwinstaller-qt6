#pragma once

namespace PatchMaker {
    // NOTE: internal class
    static class MakeDiff {
    public:
        static void create(byte[] oldBuf, byte[] newBuf, long signature, Stream output);

    private:
        const int HEADERSIZE = Diff.HEADERSIZE;

        static long readInt64(byte* buf);
        static Stream getEncodingStream(Stream stream, long signature, bool output);
        static byte[] convertPatch(byte* pPatch, long length, long inputSig, long outputSig);
        static int compareBytes(byte* left, int leftLength, byte* right, int rightLength);
        static int matchLength(byte* oldData, int oldLength, byte* newData, int newLength);
        static int search(int* I, byte* oldData, int oldLength, byte* newData, int newLength, int newOffset, int start, int end, out int pos);
        static void writeInt64(long y, byte* pb);
    }
}
