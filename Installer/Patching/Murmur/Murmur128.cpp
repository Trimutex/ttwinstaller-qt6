#include "Murmur128.hpp"

namespace TaleOfTwoWastelandsPatchingMurmur {
    uint Seed(void) {
        return _m_seed;
    }

    static Murmur128 CreateMurmur(void) {
        return new Murmur128UnmanagedX64();
    }
}
