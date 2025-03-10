#pragma once

namespace TaleOfTwoWastelandsPatching {
    // NOTE: public class
    // NOTE: static class
    class Extensions {
    public:
        // WARN: prob needs to be protected and using friend classes
        // due to vigorous usage of internal originally 
        static unsigned toUInt32(this uint8_t[] data, int start = 0);
        static unsigned toUInt32(this uint8_t[] data, int start = 0);
        static unsigned long toUInt64(this uint8_t[] data, int start = 0);
        static unsigned rotateLeft(this unsigned x, uint8_t r);
        static unsigned long rotateLeft(this unsigned long x, uint8_t r);
        static unsigned fMix(this unsigned h);
        static unsigned long fMix(this unsigned long h);

    }
}
