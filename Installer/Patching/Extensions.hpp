#pragma once

namespace TaleOfTwoWastelandsPatching {
    // NOTE: public class
    // NOTE: static class
    class Extensions {
    public:
        // WARN: prob needs to be protected and using friend classes
        // due to vigorous usage of internal originally 
        static uint toUInt32(this byte[] data, int start = 0);
        static uint toUInt32(this byte[] data, int start = 0) {
        static ulong toUInt64(this byte[] data, int start = 0)
        static uint rotateLeft(this uint x, byte r)
        static ulong rotateLeft(this ulong x, byte r)
        static uint fMix(this uint h)
        static ulong fMix(this ulong h)

    }
}
