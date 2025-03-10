#pragma once

#include <sstream>

namespace PatchMaker {
    // NOTE: internal class
    static class MakeDiff {
    public:
        static void create(uint8_t[] oldBuf, uint8_t[] newBuf, long signature, std::ostringstream output);

    private:
        const int HEADERSIZE = Diff.HEADERSIZE;

        static long readInt64(uint8_t* buf);
        static std::ostringstream getEncodingStream(std::ostringstream stream, long signature, bool output);
        static uint8_t[] convertPatch(uint8_t* pPatch, long length, long inputSig, long outputSig);
        static int compareBytes(uint8_t* left, int leftLength, uint8_t* right, int rightLength);
        static int matchLength(uint8_t* oldData, int oldLength, uint8_t* newData, int newLength);
        static int search(uint8_t* I, uint8_t* oldData, int oldLength, uint8_t* newData, int newLength, int newOffset, int start, int end, out int pos);
        static void writeInt64(long y, uint8_t* pb);
    }
}
