#pragma once

namespace TaleOfTwoWastelandsPatching {
    // NOTE: public class
    // NOTE: static class
    class Extensions {
    public:
        // WARN: prob needs to be protected and using friend classes
        // due to vigorous usage of internal originally 
        static uint ToUInt32(this byte[] data, int start = 0);
        static uint ToUInt32(this byte[] data, int start = 0) {
        static ulong ToUInt64(this byte[] data, int start = 0)
        static uint RotateLeft(this uint x, byte r)
        static ulong RotateLeft(this ulong x, byte r)
        static uint FMix(this uint h)
        static ulong FMix(this ulong h)

    }
}
