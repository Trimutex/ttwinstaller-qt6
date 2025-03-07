#include "Murmur128.hpp"

namespace TaleOfTwoWastelandsPatchingMurmur {
    uint seed(void) {
        return _m_seed;
    }

    static Murmur128 createMurmur(void) {
        return new Murmur128UnmanagedX64();
    }
}
